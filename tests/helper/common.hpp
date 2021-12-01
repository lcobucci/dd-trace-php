// Unless explicitly stated otherwise all files in this repository are
// dual-licensed under the Apache-2.0 License or BSD-3-Clause License.
//
// This product includes software developed at Datadog (https://www.datadoghq.com/).
// Copyright 2021 Datadog, Inc.
#pragma once

#include <chrono>
#include <cstddef>
#include <gmock/gmock.h>
#include <gtest/gtest.h>
#include <limits>
#include <stdexcept>
#include <sstream>
#include <string>
#include <string_view>

using ::testing::ByRef;
using ::testing::DoAll;
using ::testing::ElementsAre;
using ::testing::Return;
using ::testing::SaveArg;
using ::testing::SetArgPointee;
using ::testing::SetArgReferee;
using ::testing::Throw;
using ::testing::WithArg;
using ::testing::_;

using namespace std::literals;
using namespace std::chrono_literals;

std::string create_sample_rules_ok();
