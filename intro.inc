\section{Introduction}

ALPINE is a multi-institution effort funded by the U.S. Department of Energy's
Exascale Computing Project (ECP)~\cite{messina2017exascale}.
%
Its purpose is to deliver in situ infrastructure for visualization and
analysis to ECP application teams.
%
ALPINE's strategy is to support two existing in situ runtimes: 
VisIt's~\cite{VisIt} LibSim~\cite{LibSim} and ParaView's~\cite{ParaView} 
Catalyst~\cite{Catalyst}.
%
However, ALPINE has also developed a new in situ 
runtime, which is called ``ALPINE Ascent".
%which is also called ALPINE (and disambiguated as 
%``ALPINE in situ infrastructure" when discussed alongside the ALPINE project).
% \fix{Note: We want to call the infrastructure ``Ascent", to stem confusion.}

We believe ALPINE advances the state of the art in three distinct ways:
\begin{itemize}
\item \textbf{Support for modern supercomputing architectures.}  

ALPINE was designed with modern supercomputing architectures in mind.
%
It follows a hybrid parallel strategy, meaning it has support for both distributed-memory
parallelism across nodes and shared-memory parallelism within a node.
%
The shared-memory parallel support comes through usage of VTK-m~\cite{Moreland:CGA2016},
which encourages algorithm development using hardware-agnostic building blocks.
These building blocks
are replaced at compile time with efficient hardware-specific implementations, 
enabling portable performance over multiple architectures.
ALPINE's distributed-memory parallel support can come from either DIY~\cite{DIY} or MPI~\cite{MPI}.
%
ALPINE achieves this hybrid parallelism through use of a new library, called VTK-h (`h' for hybrid 
parallelism), that combines VTK-m and DIY/MPI.
VTK-h is  introduced later in this paper.
\item \textbf{Flyweight infrastructure.}
For ALPINE, the flyweight goal is realized in three ways: (1) an interface that
can easily be adopted by stakeholders, (2) minimal dependencies on other
software packages and small encumbrance on the binary size  of the simulation code,
and (3) minimal overheads incurred during processing, specifically with respect
to copying data and memory usage.
\item \textbf{Interoperability with software.}  
Although VTK-m plays a special role in the ALPINE project, the new Ascent runtime was designed to support additional
libraries.
%
Specifically Ascent makes use of a data flow library called ``Flow" to organize execution.
%
Flow is agnostic to the data models and libraries used in filters, and therefore can enable 
other libraries (such as R~\cite{team2000r}) to be used within Ascent.
%
Of course, it would be up to those libraries to provide support for parallelism, and additional
work would be needed to bridge between data models (for example, VTK-m to R or vice-versa).
\end{itemize}

%Contributions\fix{placeholder language}:
%\begin{itemize}
%\item First in situ library designed with a fully hybrid distributed-memory, shared-memory parallel architecture that runs portably on many-core architectures. 
%\item Flexible in situ framework that easily enables custom analysis (filters). \fix{weave filters from different runtimes together}
%\item Light weight integrations and a minimally expressive api (most operations have defaults).
%\end{itemize}

The paper is organized as follows:
Section~\ref{sec:interface} describes ALPINE's interface concepts, Section~\ref{sec:architecture}
describes the main components of ALPINE's infrastructure, and Section~\ref{sec:results} describes some initial results.

%related work

%staying tru to the requirements of strawman...

