\section{Related Work}

In situ processing has become increasingly popular in recent years.
%
Bauer et al. surveyed key research and infrastructures and we direct interested readers
to their survey~\cite{Bauer:InSitu-STAR:2016}.
%
Many of the design decisions made by ALPINE are similar to LibSim~\cite{LibSim} and Catalyst~\cite{Catalyst}: 
linking into the simulation binary, alternating execution between simulation and visualization,
and sharing the same memory space.
%
This contrasts with an approach like ADIOS~\cite{Lofstead2008}, which takes a more loosely-coupled
approach.

The ALPINE in situ infrastructure is heavily based on Strawman~\cite{LarsenStrawman}, 
with Strawman effectively serving as an early prototype for ALPINE.
%
The key differences between ALPINE and Strawman are 
(1) ALPINE is intended for end users while Strawman was a mini-app, 
(2) ALPINE has extended distributed-memory support, including usage of DIY~\cite{DIY}, 
(3) ALPINE uses VTK-m~\cite{Moreland:CGA2016} where Strawman used EAVL~\cite{EAVL}, and
(4) ALPINE has extended support for using other packages and libraries.
