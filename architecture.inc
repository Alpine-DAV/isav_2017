\section{Architecture}
\begin{figure*}
  \includegraphics[width=\textwidth=4cm]{images/alpine_arch}
  \caption{This is a tiger.}
\end{figure*}

\subsection{VTK-h}
\fix{This is paraphrased from an ECP report Hank wrote.} VTK-h is a stand alone library that implements a distributed-memory layer on top of the VTK-m library~\cite{Moreland:CGA2016}, which focuses on shared-memory parallelism.
%
The VTK-h library is a collection of distributed-memory algorithms, and VTK-h does not contain an execution model, such as the demand-driven data flow in VTK.
%
The design of VTK-h is intended to facilitate the wrapping of VTK-h algorithms so that they can be included in the execution models of other visualization tools including Alpine In Situ, ParaView, and VisIt.
%
Consequently, VTK-h serves a single point of development in which algorithms can be easily deployed into any toolkit that includes the VTK-h library.

VTK-h heavily leverages VTK-m, and the basic building block of the VTK-h data model is the VTK-m data set.
%
A VTK-h data set is a collection of VTK-m data sets along with supporting methods that handle distributed-memory queries (e.g., global scalar ranges).
%
Within VTK-h, most code will directly invoke VTK-m methods to implement algorithms, and while it is possible to directly implement new VTK-m functionality within VTK-h, that functionality is limited to distributed-memory features.
%
For distributed-memory parallelism, VTK-h uses MPI and also includes the DIY~\cite{peterka_ldav11} toolkit which encapsulates block-based abstracts that are common in distributed-memory problems.

\subsection{Flow}

\fix{Cyrus: write this section. VTK-h, by design, contains no execution model.
%
To fill this role, we created the Flow library which is included in Alpine In Situ.
%
Flow implements a simple data flow network that enables us to chain together a series of filters described by the Alpine In Situ interface.
%
Flow is generic and supports filters inputs and outputs of any type.
%
Each VTK-h algorithm is wrapped by Flow.
}

\fix{Flow makes it easy to insert custom user defined filters.}

\subsection{Ascent Runtime}
The Ascent Runtime is the layer that sits on top of Flow and beneath the Alpine In Situ Interface.
%
Ascent's responsibility is to translate a set of actions (e.g., Listings~\ref{scene_actions}, \ref{extract_actions}, and \ref{pipeline_actions} ) passed to the Alpine `Execute' method, into a Flow graph.
%
Ascent loops through hierarchy of actions contained in a conduit node, and Ascent creates a series of Flow filters (i.e., graph nodes) and connects the Flow filters together (i.e., edges).
%
Figure~\ref{flow_graph} shows the graph representation Ascent creates given the actions described by Listings~\ref{scene_actions}, \ref{extract_actions}, and \ref{pipeline_actions}.
%
When the `execute' action is processed, Ascent executes the graph.
%


\begin{figure}
\includegraphics{images/flow_graph}
\caption{\label{flow_graph}A Flow graph created from the actions described by Listings~\ref{scene_actions}, \ref{extract_actions}, and \ref{pipeline_actions}.}
\end{figure}