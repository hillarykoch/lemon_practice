// -*- mode: C++; c-indent-level: 4; c-basic-offset: 4; indent-tabs-mode: nil; -*-

#include <RcppArmadillo.h>
#include <lemon/list_graph.h>
#include "pathenumeration.h"


using namespace Rcpp;
using namespace RcppArmadillo;
using namespace lemon;
// [[Rcpp::depends(RcppArmadillo)]]


// Collect output from my algorithm
// [[Rcpp::export]]
arma::mat getPaths(){
	// need to write this function
	return arma::mat(2,2, arma::fill::ones);
}