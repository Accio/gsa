Nid <- 10000L ## genes measured
Nout <- 200L ## genes not measured
Nset <- 200
lamda.out <- 1
setMin <- 5L
setMax <- 200L
set.seed(1825)
identifiers <- sprintf("%s_%04d",
                       paste(sample(LETTERS, 4), collapse=""),
                       sample(1:Nid))
outidentifiers <- sprintf("%s_%04d",
                          paste(sample(LETTERS, 4), collapse=""),
                          sample(1:Nout))
values <- rnorm(Nid)

gss <- lapply(1:Nset, function(x) {
    ind <- sample(1:Nid,as.integer(runif(1,setMin,setMax)))
    outind <- sample(1:Nout, rpois(1, lamda.out))
    return(c(identifiers[ind], outidentifiers[outind]))
})

valTbl <- data.frame(Feat=identifiers,
                     Value=values)
write.table(valTbl, "simulated-diffstats.txt", sep="\t", row.names=FALSE)
gsGMT <- sapply(seq(along=gss), function(i)
                sprintf("SimGeneSet_%d\tDESC\t%s",
                        i, paste(gss[[i]], collapse="\t")))
writeLines(gsGMT, "simulated-genesets.gmt")
