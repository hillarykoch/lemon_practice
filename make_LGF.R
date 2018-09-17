# Make a LGF (LEMON Graph Format) file to read with LEMON

setwd("~/Desktop/lemon_practice/lemon_practice/")
library(tidyverse)
library(abind)
load("assembly_fits.Rdata")
source("assembly_lemons.R")

h <- get_h(fits)
d <- 3
path = "~/Desktop/lemon_practice/lemon_practice/lgf.txt"

make_LGF <- function(h, d, path) {
    # Make node section of LGF file
    filt_h <- filter_h(h,d)
    dims <- map(filt_h, length)
    node <- data.frame("label" = c(0,seq_along(unlist(filt_h)), 3*d+1),
                       "dim" = c(0,lapply(seq(dims), function(X) rep(X, times = dims[[X]])) %>% unlist, d+1),
                       "assoc" = c(0,unlist(filt_h),0))
    write_tsv(data.frame("@nodes"), path = path, col_names = FALSE)
    write_tsv(node, path = path, col_names = TRUE, append = TRUE)
    cat("\n", file = path, append = TRUE)

    # Make arc section of LGF file    
    consec <- get_consecutive(h)
    
    src <- trg <- list()
    for(i in seq_along(consec)) {
        src[[i]] <- sapply(consec[[i]][,1], function(X) node$label[node$assoc == X & node$dim == i])
        trg[[i]] <- sapply(consec[[i]][,2], function(X) node$label[node$assoc == X & node$dim == i+1])
    }
    
    sources <- dplyr::filter(node, dim == 1)$label
    targets <- dplyr::filter(node, dim == d)$label
    arcs <- data.frame("source" = c(rep(0, length(sources)),
                                        abind(src),
                                    targets),
                       "target" = c(sources,
                                    abind(trg),
                                    rep(3*d+1, length(targets))))
    write_tsv(data.frame("@arcs"), path = path, col_names = FALSE, append = TRUE)
    cat("\t\t -\n", file = path, append = TRUE)
    write_tsv(arcs, path = path, col_names = FALSE, append = TRUE, na = "")
    
    # Make attributes section of LGF file to note the sources
    cat("\n", file = path, append = TRUE)
    write_tsv(data.frame("@attributes"), path = path, col_names = FALSE, append = TRUE)
    attrib <- data.frame("type" = c("source", "target"),
                         "label" = c(0, 3*d+1))
    write_tsv(attrib, path = path, col_names = FALSE, append = TRUE)
}

make_LGF(h,d,path)
