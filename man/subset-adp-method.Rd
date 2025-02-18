% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/adp.R
\name{subset,adp-method}
\alias{subset,adp-method}
\title{Subset an ADP Object}
\usage{
\S4method{subset}{adp}(x, subset, ...)
}
\arguments{
\item{x}{an \linkS4class{adp} object.}

\item{subset}{A condition to be applied to the \code{data} portion of
\code{x}.  See \dQuote{Details}.}

\item{...}{Ignored.}
}
\value{
An \linkS4class{adp} object.
}
\description{
Subset an adp (acoustic Doppler profile) object, in a manner that is function
is somewhat analogous to \code{\link[=subset.data.frame]{subset.data.frame()}}.
}
\details{
For any data type,
subsetting can be by \code{time}, \code{ensembleNumber}, or \code{distance}.
These may not be combined, but it is easy to use a string of calls to
carry out combined operations, e.g.
\code{subset(subset(adp,distance<d0), time<t0)}
}
\examples{
library(oce)
data(adp)
# 1. Look at first part of time series, organized by time
earlyTime <- subset(adp, time < mean(range(adp[["time"]])))
plot(earlyTime)

# 2. Look at first ten ensembles (AKA profiles)
en <- adp[["ensembleNumber"]]
firstTen <- subset(adp, ensembleNumber < en[11])
plot(firstTen)

}
\seealso{
Other things related to adp data: 
\code{\link{[[,adp-method}},
\code{\link{[[<-,adp-method}},
\code{\link{ad2cpCodeToName}()},
\code{\link{ad2cpHeaderValue}()},
\code{\link{adp-class}},
\code{\link{adpAd2cpFileTrim}()},
\code{\link{adpConvertRawToNumeric}()},
\code{\link{adpEnsembleAverage}()},
\code{\link{adpFlagPastBoundary}()},
\code{\link{adpRdiFileTrim}()},
\code{\link{adp_rdi.000}},
\code{\link{adp}},
\code{\link{applyMagneticDeclination,adp-method}},
\code{\link{as.adp}()},
\code{\link{beamName}()},
\code{\link{beamToXyzAdpAD2CP}()},
\code{\link{beamToXyzAdp}()},
\code{\link{beamToXyzAdv}()},
\code{\link{beamToXyz}()},
\code{\link{beamUnspreadAdp}()},
\code{\link{binmapAdp}()},
\code{\link{enuToOtherAdp}()},
\code{\link{enuToOther}()},
\code{\link{handleFlags,adp-method}},
\code{\link{is.ad2cp}()},
\code{\link{plot,adp-method}},
\code{\link{read.adp.ad2cp}()},
\code{\link{read.adp.nortek}()},
\code{\link{read.adp.rdi}()},
\code{\link{read.adp.sontek.serial}()},
\code{\link{read.adp.sontek}()},
\code{\link{read.adp}()},
\code{\link{read.aquadoppHR}()},
\code{\link{read.aquadoppProfiler}()},
\code{\link{read.aquadopp}()},
\code{\link{rotateAboutZ}()},
\code{\link{setFlags,adp-method}},
\code{\link{subtractBottomVelocity}()},
\code{\link{summary,adp-method}},
\code{\link{toEnuAdp}()},
\code{\link{toEnu}()},
\code{\link{velocityStatistics}()},
\code{\link{xyzToEnuAdpAD2CP}()},
\code{\link{xyzToEnuAdp}()},
\code{\link{xyzToEnu}()}

Other functions that subset oce objects: 
\code{\link{subset,adv-method}},
\code{\link{subset,amsr-method}},
\code{\link{subset,argo-method}},
\code{\link{subset,cm-method}},
\code{\link{subset,coastline-method}},
\code{\link{subset,ctd-method}},
\code{\link{subset,echosounder-method}},
\code{\link{subset,lobo-method}},
\code{\link{subset,met-method}},
\code{\link{subset,oce-method}},
\code{\link{subset,odf-method}},
\code{\link{subset,rsk-method}},
\code{\link{subset,sealevel-method}},
\code{\link{subset,section-method}},
\code{\link{subset,topo-method}},
\code{\link{subset,xbt-method}}
}
\author{
Dan Kelley
}
\concept{functions that subset oce objects}
\concept{things related to adp data}
