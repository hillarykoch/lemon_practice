# Functions needed to process fconstr_pGMCM to then dump into an LGF file

# Get the 2-mers from the fits
get_h <- function(fits) {
    mus <- purrr::map(fits, "mu") %>%
        lapply(function(X)
            replace(X, X > 0, 1)) %>%
        lapply(function(X)
            replace(X, X < 0,-1))
    mus
}

# Find out which associations exist in each dimension
# (may be all, but if filterable, can make problem easier)
filter_h <- function(h, d) {
    spl <- strsplit(names(h), split = "_") %>%
        purrr::map(as.numeric)
    
    filt <- list()
    for (i in seq(d)) {
        lapply(seq_along(spl), function(X) {
            idx <- spl[[X]] == i
            if (any(idx)) {
                h[[X]][, idx]
            }
        }) %>%
            unlist %>%
            unique %>%
            sort -> filt[[i]]
    }
    names(filt) <- seq(d)
    filt
}

# Extract which results correspond to consecutive dimensions (e.g. 1_2, 2_3, but not 1_3)
get_consecutive <- function(h) {
    nms <- names(h)
    consec <- strsplit(names(h), split = "_") %>%
        purrr::map(as.numeric) %>%
        map(diff) %>%
        `==`(1)
    h[consec]
}