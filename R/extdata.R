#' Sample ctd dataset in AML format
#'
#' This file may be read with [read.ctd.aml()].  It is based
#' on a file donated by Ashley Stanek, which was shortened to
#' just 50 points for inclusion in oce, and which had some
#' identifying information (serial number, IP address, and WEP
#' code) zeroed-out.
#'
#' @name ctd_aml.csv.gz
#'
#' @docType data
#'
#' @encoding UTF-8
#'
#' @examples
#' ctd <- read.ctd.aml(system.file("extdata", "ctd_aml.csv.gz", package="oce"))
#' summary(ctd)
#' plot(ctd)
#'
#' @family raw datasets
#' @family things related to ctd data
NULL

#' Sample ctd dataset in odf format
#'
#' The location is approximately 30km southeast of Halifax Harbour,
#' at "Station 2" of the Halifax Line on the Scotian Shelf.
#'
#' @name CTD_BCD2014666_008_1_DN.ODF.gz
#'
#' @docType data
#'
#' @encoding UTF-8
#'
#' @examples
#' ctd <- read.ctd(system.file("extdata", "CTD_BCD2014666_008_1_DN.ODF.gz", package="oce"))
#' plot(ctd)
#'
#' @family raw datasets
#' @family things related to ctd data
#' @family things related to odf data
NULL


#' Sample adp (acoustic-doppler profiler) file in RDI format
#'
#' @name adp_rdi.000
#'
#' @docType data
#'
#' @examples
#'\dontrun{
#' read.oce(system.file("extdata", "adp_rdi.000", package="oce"))
#'}
#'
#' @family raw datasets
#' @family things related to adp data
NULL


#' Sample ctd dataset in .cnv format
#'
#' @name ctd.cnv.gz
#'
#' @docType data
#'
#' @encoding UTF-8
#'
#' @examples
#'\dontrun{
#' read.oce(system.file("extdata", "ctd.cnv.gz", package="oce"))
#'}
#'
#' @family raw datasets
#' @family things related to ctd data
NULL


#' Sample ctd dataset in .ctd format
#'
#' @name d200321-001.ctd.gz
#'
#' @docType data
#'
#' @encoding UTF-8
#'
#' @examples
#'\dontrun{
#' read.oce(system.file("extdata", "d200321-001.ctd.gz", package="oce"))
#'}
#'
#' @family raw datasets
#' @family things related to ctd data
NULL


#' Sample ctd dataset in .cnv format
#'
#' @name d201211_0011.cnv.gz
#'
#' @docType data
#'
#' @encoding UTF-8
#'
#' @examples
#'\dontrun{
#' read.oce(system.file("extdata", "d201211_0011.cnv.gz", package="oce"))
#'}
#'
#' @family raw datasets
#' @family things related to ctd data
NULL

#' Sample xbt dataset
#'
#' @name xbt.edf
#'
#' @docType data
#'
#' @encoding UTF-8
#'
#' @examples
#'\dontrun{
#' xbt <- read.oce(system.file("extdata", "xbt.edf", package="oce"))
#'}
#'
#' @family raw datasets
#' @family things related to xbt data
NULL
