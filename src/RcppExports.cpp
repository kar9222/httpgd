// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <Rcpp.h>

using namespace Rcpp;

// httpgd_
bool httpgd_(std::string host, int port, std::string bg, double width, double height, double pointsize, Rcpp::List aliases, bool cors, std::string token, bool webserver, bool silent);
RcppExport SEXP _httpgd_httpgd_(SEXP hostSEXP, SEXP portSEXP, SEXP bgSEXP, SEXP widthSEXP, SEXP heightSEXP, SEXP pointsizeSEXP, SEXP aliasesSEXP, SEXP corsSEXP, SEXP tokenSEXP, SEXP webserverSEXP, SEXP silentSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< std::string >::type host(hostSEXP);
    Rcpp::traits::input_parameter< int >::type port(portSEXP);
    Rcpp::traits::input_parameter< std::string >::type bg(bgSEXP);
    Rcpp::traits::input_parameter< double >::type width(widthSEXP);
    Rcpp::traits::input_parameter< double >::type height(heightSEXP);
    Rcpp::traits::input_parameter< double >::type pointsize(pointsizeSEXP);
    Rcpp::traits::input_parameter< Rcpp::List >::type aliases(aliasesSEXP);
    Rcpp::traits::input_parameter< bool >::type cors(corsSEXP);
    Rcpp::traits::input_parameter< std::string >::type token(tokenSEXP);
    Rcpp::traits::input_parameter< bool >::type webserver(webserverSEXP);
    Rcpp::traits::input_parameter< bool >::type silent(silentSEXP);
    rcpp_result_gen = Rcpp::wrap(httpgd_(host, port, bg, width, height, pointsize, aliases, cors, token, webserver, silent));
    return rcpp_result_gen;
END_RCPP
}
// httpgd_state_
Rcpp::List httpgd_state_(int devnum);
RcppExport SEXP _httpgd_httpgd_state_(SEXP devnumSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< int >::type devnum(devnumSEXP);
    rcpp_result_gen = Rcpp::wrap(httpgd_state_(devnum));
    return rcpp_result_gen;
END_RCPP
}
// httpgd_random_token_
std::string httpgd_random_token_(int len);
RcppExport SEXP _httpgd_httpgd_random_token_(SEXP lenSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< int >::type len(lenSEXP);
    rcpp_result_gen = Rcpp::wrap(httpgd_random_token_(len));
    return rcpp_result_gen;
END_RCPP
}
// httpgd_svg_
std::string httpgd_svg_(int devnum, int page, double width, double height);
RcppExport SEXP _httpgd_httpgd_svg_(SEXP devnumSEXP, SEXP pageSEXP, SEXP widthSEXP, SEXP heightSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< int >::type devnum(devnumSEXP);
    Rcpp::traits::input_parameter< int >::type page(pageSEXP);
    Rcpp::traits::input_parameter< double >::type width(widthSEXP);
    Rcpp::traits::input_parameter< double >::type height(heightSEXP);
    rcpp_result_gen = Rcpp::wrap(httpgd_svg_(devnum, page, width, height));
    return rcpp_result_gen;
END_RCPP
}
// httpgd_remove_
bool httpgd_remove_(int devnum, int page);
RcppExport SEXP _httpgd_httpgd_remove_(SEXP devnumSEXP, SEXP pageSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< int >::type devnum(devnumSEXP);
    Rcpp::traits::input_parameter< int >::type page(pageSEXP);
    rcpp_result_gen = Rcpp::wrap(httpgd_remove_(devnum, page));
    return rcpp_result_gen;
END_RCPP
}
// httpgd_clear_
bool httpgd_clear_(int devnum);
RcppExport SEXP _httpgd_httpgd_clear_(SEXP devnumSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< int >::type devnum(devnumSEXP);
    rcpp_result_gen = Rcpp::wrap(httpgd_clear_(devnum));
    return rcpp_result_gen;
END_RCPP
}
// dummygd_
bool dummygd_();
RcppExport SEXP _httpgd_dummygd_() {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    rcpp_result_gen = Rcpp::wrap(dummygd_());
    return rcpp_result_gen;
END_RCPP
}

static const R_CallMethodDef CallEntries[] = {
    {"_httpgd_httpgd_", (DL_FUNC) &_httpgd_httpgd_, 11},
    {"_httpgd_httpgd_state_", (DL_FUNC) &_httpgd_httpgd_state_, 1},
    {"_httpgd_httpgd_random_token_", (DL_FUNC) &_httpgd_httpgd_random_token_, 1},
    {"_httpgd_httpgd_svg_", (DL_FUNC) &_httpgd_httpgd_svg_, 4},
    {"_httpgd_httpgd_remove_", (DL_FUNC) &_httpgd_httpgd_remove_, 2},
    {"_httpgd_httpgd_clear_", (DL_FUNC) &_httpgd_httpgd_clear_, 1},
    {"_httpgd_dummygd_", (DL_FUNC) &_httpgd_dummygd_, 0},
    {NULL, NULL, 0}
};

RcppExport void R_init_httpgd(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
