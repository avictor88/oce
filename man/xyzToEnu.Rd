% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/ad.R
\name{xyzToEnu}
\alias{xyzToEnu}
\title{Convert Acoustic-Doppler Data From xyz to enu Coordinates}
\usage{
xyzToEnu(x, ...)
}
\arguments{
\item{x}{an \linkS4class{adp} or \linkS4class{adv} object.}

\item{\dots}{extra arguments that are passed on to \code{\link[=xyzToEnuAdp]{xyzToEnuAdp()}}
or \code{\link[=xyzToEnuAdv]{xyzToEnuAdv()}}; see the documentation for those functions,
for the details.}
}
\value{
An object of the same class as \code{x}, but with velocities
in east-north-up coordinates instead of xyz coordinates.
}
\description{
Convert Acoustic-Doppler Data From xyz to enu Coordinates
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
\code{\link{subset,adp-method}},
\code{\link{subtractBottomVelocity}()},
\code{\link{summary,adp-method}},
\code{\link{toEnuAdp}()},
\code{\link{toEnu}()},
\code{\link{velocityStatistics}()},
\code{\link{xyzToEnuAdpAD2CP}()},
\code{\link{xyzToEnuAdp}()}

Other things related to adv data: 
\code{\link{[[,adv-method}},
\code{\link{[[<-,adv-method}},
\code{\link{adv-class}},
\code{\link{advSontekAdrFileTrim}()},
\code{\link{adv}},
\code{\link{applyMagneticDeclination,adv-method}},
\code{\link{beamName}()},
\code{\link{beamToXyz}()},
\code{\link{enuToOtherAdv}()},
\code{\link{enuToOther}()},
\code{\link{plot,adv-method}},
\code{\link{read.adv.nortek}()},
\code{\link{read.adv.sontek.adr}()},
\code{\link{read.adv.sontek.serial}()},
\code{\link{read.adv.sontek.text}()},
\code{\link{read.adv}()},
\code{\link{rotateAboutZ}()},
\code{\link{subset,adv-method}},
\code{\link{summary,adv-method}},
\code{\link{toEnuAdv}()},
\code{\link{toEnu}()},
\code{\link{velocityStatistics}()},
\code{\link{xyzToEnuAdv}()}
}
\concept{things related to adp data}
\concept{things related to adv data}
