\section{Interface}
The top-level interface to Alpine mirrors the Strawman interface and it consists of four functions: Open, Publish, Execute, and Close.
%
All data passed into Alpine using Conduit~\cite{conduit}, a library that provides intuitive model for describing hierarchical scientific data.
% 
Alpine is initialized with the ``Open'' call with several parameters including an MPI communicator.
%
Next, simulation mesh data structures are described using Conduit's Blueprint interface, which is a simple set of conventions for describing  mesh data ranging from uniform meshes to topologies composed of higher order elements, and the resulting Conduit node is published through the ``Publish'' call.
%
Third, Alpine executes a series of user defined actions through the ``Execute'' call.
%
Finally, the ``Close'' call informs Alpine that no further execution is required.

\begin{lstlisting}[caption={Top-level Alpine interface.},captionpos=b,label=top_level_interface]
Alpine alpine;
alpine.Open(options);
alpine.Publish(data);
alpine.Execute(actions);
alpine.Close();
\end{lstlisting}

\subsection{Actions API}

We designed the \fix{actions interface} to support three main use cases: making pictures, capturing data, and transforming data.
%
To support these use cases, we leverage three abstractions:
\begin{itemize}
\item Scenes: make pictures
\item Extracts: capture data
\item Pipelines: transform data
\end{itemize}
At a high level, Scenes allow uses to describe the images they want to create.
%
Extracts allows uses to describe how they want to capture data, and Pipelines allows users to describe how they want to transform their data.

\subsection{Scenes: Make Pictures}
\label{sec:scenes}
A scene encapsulates the required information to generate one or more images.
%
The user specifies a collection of plots (e.g., volume or surface rendering) and associated parameters such as camera definitions, light positions, and annotation types.
%

There are only two required parameters for defining a scene: plot type and scalar field.
%
A scene defined in this way uses the default data source, which is all of the data published by the simulation, and the scene uses the default settings for camera position, image resolution, lighting, and annotations.
%
An example of a scene defined with only the required parameters is shown at the beginning of Listing~\ref{scene_actions}.
%
The definition of ``scene1'' indicates that a volume plot of the scalar field ``noise'' using the default parameters.
%
In this case, a single image of the volume plot of the scalar field ``noise'' will be save to the file system.

\begin{lstlisting}[caption={Scene descriptions.},captionpos=b,label=scene_actions]
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
Unlike the first example, the plots that form ``scene2'' do not use the default data source, and they instead will uses the result of a pipeline (see Section~\ref{sec:pipelines}), a series of transformations on the published mesh data. 

A single scene definition can create more than one image.
%
The rendering parameters are contained in a list, and one image is created for each entry.
%
Further, rendering parameters support camera definitions that output many images, such as Cinema~\ref{AhrensCinema}, where the camera has parameters $phi$ and $theta$ and the total images produced is $phi * theta$.


\subsection{Extracts: Capture Data}
\label{sec:extracts}

Extracts are an abstraction that enables the use to specify how they want to capture their data.
%
In terms of Alpine, data capture sends data outside the Alpine infrastructure.
%
Examples include writing out the raw simulation data to the file system, creating HDF5 files, or sending the data off node (e.g., ADIOS~\cite{Lofstead2008}).

\begin{lstlisting}[caption={Extract descriptions.},captionpos=b,label = extract_actions]
conduit::Node extracts;
// use default pipeline (original mesh)
extracts["ex1/type"] = "hdf5";

// use the result of a pipeline 
extracts["ex2/type"] = "hdf5";
extracts["ex2/pipeline"] = "pl2";
\end{lstlisting}

\subsection{Pipelines: Transform Data}
\label{sec:pipelines}

Extracts, like scenes, can either consume the default data source, all published simulation data, or the result of a pipeline (see~\ref{sec:pipelines}).
%
Listing~\ref{extract_actions} show the declaration of two extracts.
%
The first extract saves the default data source into an HDF5 file, and the second extract saves the result of a pipeline, referenced by pipeline name, into an HD5f file.

\begin{lstlisting}[caption={Pipeline desctiptions.},captionpos=b,label=pipeline_actions]
conduit::Node pipelines;
// pipeline 1
pipelines["pl1/f1/type"] = "contour";
// filter knobs
conduit::Node contour_params; 
contour_params["field"] = "noise";
contour_params["iso_values"] = {0.0, 0.5};
pipelines["pl1/f1/params"] = contour_params;

// pipeline 2 
pipelines["pl2/f1/type"] = "threshold";
// filter knobs
conduit::Node thresh_params;
thresh_params["field"]  = "noise";
thresh_params["min_value"] = 0.0;
thresh_params["max_value"] = 0.5;
pipelines["pl2/f1/params"] = thresh_params;

pipelines["pl2/f2/type"]   = "clip";
// filter knobs
conduit::Node clip_params; 
clip_params["topology"] = "mesh";
clip_params["sphere/center/x"] = 0.0;
clip_params["sphere/center/y"] = 0.0;
clip_params["sphere/center/z"] = 0.0;
clip_params["sphere/radius"]   = .1;
pipelines["pl2/f2/params/"] = clip_params;
\end{lstlisting}