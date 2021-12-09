// Unless explicitly stated otherwise all files in this repository are
// dual-licensed under the Apache-2.0 License or BSD-3-Clause License.
//
// This product includes software developed at Datadog
// (https://www.datadoghq.com/). Copyright 2021 Datadog, Inc.
#ifndef ENGINE_HPP
#define ENGINE_HPP
#include "config.hpp"
#include "parameter.hpp"
#include "result.hpp"
#include "subscriber/base.hpp"
#include <iostream>
#include <map>
#include <memory>
#include <spdlog/fmt/ostr.h>
#include <string>
#include <vector>

namespace dds {

/**
 * Semantics:
 *    - engine: pub/sub broker, provides subscription framework.
 *    - engine::context: request-bound broker, provides publishing framework.
 *    - subscriber: data consumer, defines its required topics and immutable
 *                  state.
 *    - subscriber::listener: request-bound data consumer, consumes the data and
 *                            provides the mutable state.
 *    - address: addresses to which a subscriber subscribes.
 *    - subscription: the mapping between an address and a subscriber.
 **/
class engine : std::enable_shared_from_this<engine> {
    engine() = default;

public:
    static constexpr int default_waf_timeout_ms = 10;

    struct settings {
        std::string rules_file;
        std::uint64_t waf_timeout_ms = default_waf_timeout_ms;

        bool operator==(const settings &oth) const noexcept
        {
            return rules_file == oth.rules_file &&
                   waf_timeout_ms == oth.waf_timeout_ms;
        }

        friend auto &operator<<(std::ostream &os, const settings &c)
        {
            return os << "{rules_file=" << c.rules_file
                      << ", waf_timeout_ms=" << c.waf_timeout_ms << "}";
        }

        struct settings_hash {
            std::size_t operator()(const settings &s) const noexcept
            {
                auto h1 = std::hash<decltype(rules_file)>{}(s.rules_file);
                auto h2 =
                    std::hash<decltype(waf_timeout_ms)>{}(s.waf_timeout_ms);
                return h1 ^ h2;
            }
        };
    };

    using subscription_map =
        std::map<std::string_view, std::vector<subscriber::ptr>>;

    // Assuming here the callers are nice enough that context doesn't live
    // beyond the engine. This could be enforced by having the context
    // store a shared_ptr to the engine
    class context {
    public:
        explicit context(const engine &engine)
            : subscriptions_(engine.subscriptions_)
        {}
        context(const context &) = delete;
        context &operator=(const context &) = delete;
        context(context &&) = delete;
        context &operator=(context &&) = delete;
        ~context();

        result publish(parameter &&param);

    protected:
        std::vector<parameter> prev_published_params_;
        std::map<subscriber::ptr, subscriber::listener::ptr> listeners_;
        const subscription_map &subscriptions_;
    };

    static auto create() { return std::shared_ptr<engine>(new engine()); }

    context get_context() { return context{*this}; }
    void subscribe(const subscriber::ptr &sub);

protected:
    subscription_map subscriptions_;
};

} // namespace dds

#endif // ENGINE_HPP
