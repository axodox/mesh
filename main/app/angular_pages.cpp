#include "angular_pages.hpp"
#include "infrastructure/bitwise_operations.hpp"
#include "infrastructure/dependencies.hpp"
#include "networking/http_server.hpp"

using namespace mesh::infrastructure;
using namespace mesh::networking;

namespace mesh::app
{
  define_file(file_favicon_png, "_binary_favicon_png_gz_start", "_binary_favicon_png_gz_end");
  define_file(file_index_html, "_binary_index_html_gz_start", "_binary_index_html_gz_end");
  define_file(file_main_js, "_binary_main_js_gz_start", "_binary_main_js_gz_end");
  define_file(file_polyfills_js, "_binary_polyfills_js_gz_start", "_binary_polyfills_js_gz_end");
  define_file(file_runtime_js, "_binary_runtime_js_gz_start", "_binary_runtime_js_gz_end");
  define_file(file_styles_css, "_binary_styles_css_gz_start", "_binary_styles_css_gz_end");

  angular_pages::angular_pages()
  {
    auto server = dependencies.resolve<http_server>();

    const auto static_file_options = http_static_file_options::compress_gzip;
    server->add_static_file("/favicon.png", mime_type::png, file_favicon_png, static_file_options);
    server->add_static_file("/index.html", mime_type::html, file_index_html, bitwise_or(static_file_options, http_static_file_options::redirect_root));
    server->add_static_file("/main.js", mime_type::js, file_main_js, static_file_options);
    server->add_static_file("/polyfills.js", mime_type::js, file_polyfills_js, static_file_options);
    server->add_static_file("/runtime.js", mime_type::js, file_runtime_js, static_file_options);
    server->add_static_file("/styles.css", mime_type::css, file_styles_css, static_file_options);
  }
}