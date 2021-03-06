\section{Infrastructure}
\label{sec:architecture}
% \begin{figure}
%   \includegraphics[width=\linewidth]{images/alpine_arch}
%   \caption{ALPINE Interface and ALPINE Ascent Architecture}
% \end{figure}

VTK-h, Flow, and Ascent are the three main ALPINE in situ infrastructure software components. VTK-h is a library that will enable development of in situ algorithms that can be deployed in VisIt, ParaView, and Ascent. Flow is a simple data flow library that orchestrates the setup and execution of filter graphs. ALPINE Ascent is a new flyweight in situ runtime that implements the actions supported by the ALPINE Interface. Ascent leverages VTK-h and Flow. This section describes the roles of VTK-h, Flow, and Ascent.


\subsection{VTK-h}
%\fix{This is paraphrased from an ECP report Hank wrote.}
VTK-h is a stand alone library that implements a distributed-memory layer on top of the VTK-m library~\cite{Moreland:CGA2016}, which focuses on shared-memory parallelism.
%
The VTK-h library is a collection of distributed-memory algorithms, and VTK-h does not contain an execution model, such as the demand-driven data flow in VTK.
%
The design of VTK-h is intended to facilitate the wrapping of VTK-m algorithms so that they can be included in the execution models of other visualization tools including ALPINE Ascent, ParaView, and VisIt.
%
Consequently, VTK-h serves as a single point of development in which algorithms can be easily deployed into any toolkit that includes the VTK-h library.

VTK-h heavily leverages VTK-m, and the basic building block of the VTK-h data model is the VTK-m data set.
%
\fix{Strawman uses EAVL for its shared-memory parallelism, and just as ALPINE Ascent is the successor to Strawman, VTK-m is the successor to EAVL.}
%
A VTK-h data set is a collection of VTK-m data sets along with supporting methods that handle distributed-memory queries (e.g., global scalar ranges).
%
Within VTK-h, most code will directly invoke VTK-m methods to implement algorithms, and while it is possible to directly implement new VTK-m functionality within VTK-h, that functionality is limited to distributed-memory features.
%
For distributed-memory parallelism, VTK-h uses MPI and also includes the DIY~\cite{peterka_ldav11} toolkit which encapsulates block-based abstractions that are common in distributed-memory problems, and VTK-h uses DIY to implement distributed-memory image compositing.

\subsection{Flow}

%
%\fix{transition is abrupt here b/c the intro sentence re-hashes a main point from the prior section.}
% 
Recall from the prior section that VTK-h does not provide its own execution model. This choice simplifies the VTK-h API and makes it easy to leverage VTK-h within ParaView and VisIt`s existing full featured execution models. 

Since ALPINE Ascent does not leverage ParaView or VisIt's infrastructure, it needs a basic execution model to support using VTK-h algorithms to carry out the user's requested actions. 

Ascent uses a simple data flow library named Flow to efficiently compose and execute VTK-h filters. ALPINE's Flow library is a C++ evolution of the Python data flow network infrastructure used in \cite{cyrush_pyhpc2012}. It supports declaration and execution of directed acyclic graphs (DAGs) of filters created from a menu of filter types that are registered at runtime. Filters declare a minimal interface, which includes the number of expected inputs and outputs, and a set of default parameters. Flow uses a topological sort to ensure proper filter execution order, tracks all intermediate results, and provides basic memory management capabilities. 

The VTK-h algorithms needed by Ascent are wrapped as Flow Filters so they can be executed as part of DAGs composed by Ascent.

Like its Python predecessor, Flow provides support for generic inputs and outputs. Flow provides a mechanism for filters to check input data types at runtime if necessary. Because of this data-type agnostic design, the Flow library does not depend on VTK-h. This provides the flexibility to create filters which can process data in other data models and APIs. This design supports important future use cases, such as creating a filter to refine high-order MFEM \cite{mfem-library} meshes into VTK-h data sets for rendering.


% \fix{Cyrus: write this section. VTK-h, by design, contains no execution model.
% %
% %
% Flow implements a simple data flow network that enables us to chain together a series of filters described by the ALPINE In Situ interface.
% %
% Flow is generic and supports filters inputs and outputs of any type.
%

% }
%
% \fix{Flow makes it easy to insert custom user defined filters.}

\subsection{Ascent Runtime}
The Ascent Runtime is the layer that sits on top of Flow and beneath the ALPINE In Situ Interface.
%
Ascent's responsibility is to translate a set of actions (e.g., Listings~\ref{scene_actions}, \ref{extract_actions}, and \ref{pipeline_actions}) passed to the ALPINE ``Execute'' method into a Flow graph.
%
Ascent loops through hierarchy of actions contained in a Conduit Node, and creates a series of Flow filters (i.e., graph nodes) and connects the Flow filters together (i.e., edges).
%
Figure~\ref{flow_graph} shows the graph representation Ascent creates given the actions described by Listings~\ref{scene_actions}, \ref{extract_actions}, and \ref{pipeline_actions}.
%
When the `execute' action is processed, Ascent executes the graph.
%


\begin{figure}
\includegraphics[width=4cm]{images/flow_graph}
\caption{\label{flow_graph}A Flow graph created from the actions described by Listings~\ref{scene_actions}, \ref{extract_actions}, and \ref{pipeline_actions}.}
\end{figure}