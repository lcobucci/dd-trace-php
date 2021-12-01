--TEST--
Test full set of ancillary tags
--INI--
ddappsec.extra_headers=,mY-header,,my_other_header
--ENV--
REQUEST_URI=/my/ur%69/
SCRIPT_NAME=/my/uri.php
PATH_INFO=/ur%69/
REQUEST_METHOD=GET
URL_SCHEME=http
HTTP_X_FORWARDED_FOR=7.7.7.7,10.0.0.1
HTTP_X_CLIENT_IP=7.7.7.7
HTTP_X_REAL_IP=7.7.7.8
HTTP_X_FORWARDED=for="foo"
HTTP_X_CLUSTER_CLIENT_IP=7.7.7.9
HTTP_FORWARDED_FOR=7.7.7.10,10.0.0.1
HTTP_FORWARDED=for="foo"
HTTP_VIA=HTTP/1.1 GWA
HTTP_TRUE_CLIENT_IP=7.7.7.11
HTTP_CONTENT_TYPE=text/plain
HTTP_CONTENT_LENGTH=0
HTTP_CONTENT_ENCODING=utf-8
HTTP_CONTENT_LANGUATE=pt-PT
HTTP_HOST=myhost:8888
HTTP_USER_AGENT=my user agent
HTTP_ACCEPT=*/*
HTTP_ACCEPT_ENCODING=gzip
HTTP_ACCEPT_LANGUAGE=pt-PT
HTTP_MY_HEADER=my header value
HTTP_MY_OTHER_HEADER=my other header value
HTTP_IGNORED_HEADER=ignored header
REMOTE_ADDR=7.7.7.12
HTTPS=on
--GET--
key=val
--FILE--
<?php

use function datadog\appsec\testing\add_ancillary_tags;

header('Content-type: application/json');
http_response_code(404);
flush();

$_SERVER = array();

$arr = array();
add_ancillary_tags($arr);
ksort($arr);
print_r($arr);

?>
--EXPECTF--
Array
(
    [http.method] => GET
    [http.request.headers.accept] => */*
    [http.request.headers.accept-encoding] => gzip
    [http.request.headers.accept-language] => pt-PT
    [http.request.headers.content-encoding] => utf-8
    [http.request.headers.content-length] => 0
    [http.request.headers.content-type] => text/plain
    [http.request.headers.forwarded] => for="foo"
    [http.request.headers.forwarded-for] => 7.7.7.10,10.0.0.1
    [http.request.headers.host] => myhost:8888
    [http.request.headers.my-header] => my header value
    [http.request.headers.my-other-header] => my other header value
    [http.request.headers.true-client-ip] => 7.7.7.11
    [http.request.headers.user-agent] => my user agent
    [http.request.headers.via] => HTTP/1.1 GWA
    [http.request.headers.x-client-ip] => 7.7.7.7
    [http.request.headers.x-cluster-client-ip] => 7.7.7.9
    [http.request.headers.x-forwarded] => for="foo"
    [http.request.headers.x-forwarded-for] => 7.7.7.7,10.0.0.1
    [http.request.headers.x-real-ip] => 7.7.7.8
    [http.response.headers.content-type] => application/json
    [http.status_code] => 404
    [http.url] => https://myhost:8888/my/uri.php
    [http.useragent] => my user agent
    [network.client.ip] => 7.7.7.12
)
