\section{Interface}
\label{sec:interface}

The ALPINE Interface is the API used to direct ALPINE Ascent, ALPINE's new flyweight in situ runtime. 

The top-level interface mirrors the Strawman interface and consists of four methods: ``Open'', ``Publish'', ``Execute'', and ``Close''.
%
All data and parameters are passed into ALPINE using Conduit~\cite{conduit}, a library that provides an intuitive model for describing hierarchical scientific data. Since Conduit provides C++, C, Fortran, and Python APIs for data description, exposing the ALPINE interface in these languages is trivial.

% 
ALPINE is initialized by calling the ``Open'' method and passing several parameters, including an MPI communicator.
%
Next, simulation mesh data structures are described using Conduit's Mesh Blueprint \cite{conduit_blueprint}, a set of conventions for describing domain-decomposed mesh data ranging from uniform meshes to topologies composed of higher order elements, and the resulting Conduit Node is published through ALPINE's ``Publish'' method. 
%
ALPINE verifies that published data conforms to the Mesh Blueprint. If the data does not conform, a detailed description of what is missing or invalid is provided. 
%
Third, ALPINE executes sets of actions described and passed to the ``Execute'' method.
%
Finally, the ``Close'' call informs ALPINE that no further execution is required.

\begin{lstlisting}[caption={Top-level ALPINE interface in C++.},captionpos=b,label=top_level_interface]
Alpine alpine;
alpine.Open(options);
alpine.Publish(data);
alpine.Execute(actions);
alpine.Close();
\end{lstlisting}

\subsection{Actions}

\begin{figure}
  \includegraphics[width=\linewidth]{images/alpine_arch}
  \caption{\label{ascent_arch} The ALPINE Interface allows users to declare Scenes, Extracts, and Pipelines for execution by the ALPINE Ascent runtime.}
\end{figure}


In ALPINE, we replaced Strawman's simple rendering focused actions with a more expressive interface. The new interface is organized around three main use cases: making pictures, capturing data, and transforming data.
%
Users invoke these use cases by passing actions that declare sets of Scenes, Extracts, and Pipelines:

\begin{itemize}
\item Scenes: make pictures
\item Extracts: capture data
\item Pipelines: transform data
\end{itemize}


At a high level, Scenes allow users to describe the images they want to create.
%
Extracts allows users to describe how they want to capture data for use outside of ALPINE, and Pipelines allow users to describe the ways they want ALPINE to transform their data. Figure~\ref{ascent_arch} outlines how three concepts are supported.

\subsubsection{Scenes: Make Pictures}
\label{sec:scenes}
A scene encapsulates the information required to generate one or more images.
%
The user specifies a collection of plots (e.g., volume or surface rendering) and associated parameters such as camera definitions, light positions, and annotations.
%

To define a scene, a user describes one or more plots. The simplest plot description requires only a plot type and scalar field name.
%
A scene defined in this way uses the default data source, which is all of the data published by the simulation, and default settings for camera position, image resolution, lighting, and annotations.
%
An example of a minimally defined scene is shown at the beginning of Listing~\ref{scene_actions}.
%
The definition of ``scene1'' indicates the user wants a volume plot of the scalar field ``noise'' using the default parameters.
%
In this case, a single image of the volume rendering of the scalar field ``noise'' will be saved to the file system.

\begin{lstlisting}[caption={Example Scene Descriptions in C++},captionpos=b,label=scene_actions]
conduit::Node scenes;
// scene 1, a single volume plot 
// w/ default camera and output res
scenes["scene1/plots/plot1/type"] = "volume";
scenes["scene1/plots/plot1/params/field"] = "noise";

// scene 2, a pc plot + mesh plot of pipeline 'pl1'
// w/ default camera and output res
scenes["scene2/plot1/type"] = "pseudocolor";
scenes["scene2/plot1/pipeline"] = "pl1";
scenes["scene2/plot1/params/field"] = "noise";

scenes["scene2/plot2/type"]     = "mesh";
scenes["scene2/plot2/pipeline"] = "pl2";
\end{lstlisting}

The second scene definition. ``scene2'', in Listing~\ref{scene_actions} indicates that a single image should be created by combining the output of two plots, a pseudocolor plot and a mesh plot.
%
Unlike the first example, the plots that form ``scene2'' do not use the default data source. Instead, they use the result of a pipeline (see Section~\ref{sec:pipelines}), a series of transformations on the published mesh data. 

A single scene definition can create more than one image.
%
The rendering parameters are contained in a list, and one image is created for each entry.
%
Further, rendering parameters support camera definitions that output many images. This allows us to target  Cinema~\cite{AhrensCinema}, by providing camera parameters $phi$ and $theta$, that produce a total of $phi * theta$ images.


\subsubsection{Extracts: Capture Data}
\label{sec:extracts}

Extracts are an abstraction that enables the user to specify how they want to capture their data.
%
In terms of ALPINE, data capture sends data outside the ALPINE infrastructure.
%
Examples include writing out the raw simulation data to the file system, creating HDF5 files, or sending the data off node (e.g., ADIOS~\cite{Lofstead2008}).

\begin{lstlisting}[caption={Example Extract Descriptions in C++.},captionpos=b,label = extract_actions]
conduit::Node extracts;
// use default pipeline (original mesh)
extracts["ex1/type"] = "hdf5";

// use the result of a pipeline 
extracts["ex2/type"] = "hdf5";
extracts["ex2/pipeline"] = "pl2";
\end{lstlisting}


Extracts, like scenes, can either consume the default data source, all published simulation data, or the result of a pipeline (see Section~\ref{sec:pipelines}).
%
Listing~\ref{extract_actions} shows the declaration of two extracts.
%
The first extract saves the default data source into an HDF5 file, and the second extract saves the result of a pipeline, referenced by pipeline name, into an HDF5 file.

\subsubsection{Pipelines: Transform Data}
\label{sec:pipelines}

Pipelines allow users to compose filters that transform the published input data into new meshes.
This is where users specify typical geometric transforms (e.g., clipping and slicing), field based transforms (e.g., threshold and contour), etc. The resulting data from each Pipeline can be used as input to Scenes or Extracts. 

\begin{lstlisting}[caption={Example Pipeline Descriptions in C++.},captionpos=b,label=pipeline_actions]
conduit::Node pipelines;
// pipeline 1
pipelines["pl1/f1/type"] = "contour";
// filter parameters 
conduit::Node contour_params; 
contour_params["field"] = "noise";
contour_params["iso_values"] = {0.0, 0.5};
pipelines["pl1/f1/params"] = contour_params;

// pipeline 2 
pipelines["pl2/f1/type"] = "threshold";
// filter parameters
conduit::Node thresh_params;
thresh_params["field"]  = "noise";
thresh_params["min_value"] = 0.0;
thresh_params["max_value"] = 0.5;
pipelines["pl2/f1/params"] = thresh_params;

pipelines["pl2/f2/type"]   = "clip";
// filter parameters
conduit::Node clip_params; 
clip_params["topology"] = "mesh";
clip_params["sphere/center/x"] = 0.0;
clip_params["sphere/center/y"] = 0.0;
clip_params["sphere/center/z"] = 0.0;
clip_params["sphere/radius"]   = .1;
pipelines["pl2/f2/params/"] = clip_params;
\end{lstlisting}

Listing~\ref{pipeline_actions} shows the declaration of two pipelines. The first applies a contour filter to extract two isosurfaces of the scalar field ``noise''. The second pipeline applies a threshold filter to screen the ``noise'' field, and then a clip filter to extract the intersection of what remains from the threshold with a sphere.
