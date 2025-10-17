
#include <cstdlib>
#include <string>
#include <iostream>
#include <chrono>
#include <curl/curl.h>

using clk = std::chrono::steady_clock;

static size_t sink(char* ptr, size_t size, size_t nmemb, void*) {
  return size * nmemb; 
}

static std::string env_or(const char *name, const char *fallback)
{
    const char *v = std::getenv(name);
    return v ? std::string(v) : std::string(fallback);
}

static long env_or_long(const char *name, long fallback)
{
    const char *v = std::getenv(name);
    return v ? std::stol(v, nullptr, 10) : fallback;
}

int main()
{
    const std::string url = env_or("URL", "https://example.org/");
    const long timeout_ms = env_or_long("TIMEOUT_MS", 5000);
    const bool http2 = env_or_long("HTTP2", 0) != 0;
    
    curl_global_init(CURL_GLOBAL_DEFAULT);
    CURL *e = curl_easy_init();
    if (!e) {
        std::cerr << "curl_easy_init failed";
        return 2;    
    }

    curl_easy_setopt(e, CURLOPT_URL, url.c_str());
    curl_easy_setopt(e, CURLOPT_WRITEFUNCTION, sink);
    curl_easy_setopt(e, CURLOPT_TIMEOUT_MS, timeout_ms);
    curl_easy_setopt(e, CURLOPT_USERAGENT, "transport-bench/0.1");
    curl_easy_setopt(e, CURLOPT_FOLLOWLOCATION, 0L);
    curl_easy_setopt(e, CURLOPT_NOSIGNAL, 1L); 

    const auto t0 = clk::now();
    CURLcode rc = curl_easy_perform(e);
    const auto t1 = clk::now();


    long code = 0;
    curl_easy_getinfo(e, CURLINFO_RESPONSE_CODE, &code);

    auto to_ms = [](double sec) { return sec * 1000.0; };
    double dns_ms = -1, conn_ms = -1, tls_ms = -1, ttfb_ms = -1, tot_ms = -1;

    curl_off_t namelookup_us = 0, connect_us = 0, appconnect_us = 0, start_xfer_us = 0, total_us = 0;
    if (CURLE_OK == curl_easy_getinfo(e, CURLINFO_NAMELOOKUP_TIME_T, &namelookup_us) &&
        CURLE_OK == curl_easy_getinfo(e, CURLINFO_CONNECT_TIME_T, &connect_us) &&
        CURLE_OK == curl_easy_getinfo(e, CURLINFO_STARTTRANSFER_TIME_T, &start_xfer_us) &&
        CURLE_OK == curl_easy_getinfo(e, CURLINFO_TOTAL_TIME_T, &total_us)) {
        dns_ms = namelookup_us / 1000.0;
        conn_ms = (connect_us - namelookup_us) / 1000.0;

    // TLS handshake time is only meaningful for HTTPS; APPCONNECT may be 0 for plain HTTP.
    if (CURLE_OK == curl_easy_getinfo(e, CURLINFO_APPCONNECT_TIME_T, &appconnect_us) && appconnect_us > 0) {
        tls_ms = (appconnect_us - connect_us) / 1000.0;
        ttfb_ms = (start_xfer_us - appconnect_us) / 1000.0;
    } else {
        ttfb_ms = (start_xfer_us - connect_us) / 1000.0;
    }
        tot_ms = total_us / 1000.0;
    }


}
