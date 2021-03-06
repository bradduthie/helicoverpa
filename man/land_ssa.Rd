% Generated by roxygen2: do not edit by hand
% Please edit documentation in R/landscape.R
\name{land_ssa}
\alias{land_ssa}
\title{land SSA}
\usage{
land_ssa(dim_x, dim_y, farms, public_land, land_var)
}
\arguments{
\item{dim_x}{The number of cells on the X dimension of the landscape}

\item{dim_y}{The number of cells on the Y dimension of the landscape}

\item{farms}{Number of farms among which landscape cells will be divided}

\item{public_land}{The amount of land that will not be owned}

\item{land_var}{Does distribution of land vary among users? >=0, <1}
}
\value{
A two dimensional array of cells with ownership values
}
\description{
Builds a layer of the landscape with a shortest-splitline algorithm to assign
landscape cells among farms
}
