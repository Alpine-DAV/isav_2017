\section{Results}
\label{sec:results}

\begin{figure}
\includegraphics[width=4cm]{images/kripke}
\caption{Image produced from Kripke.}
\end{figure}

\begin{table}[]
\centering

\label{my-label}
\begin{tabular}{ccccl}
\cline{1-4}
\multicolumn{1}{|c|}{}    & \multicolumn{1}{c|}{Sim}  & \multicolumn{1}{c|}{Vis} & \multicolumn{1}{c|}{Percentage Vis} &  \\ \cline{1-4}
\multicolumn{1}{|c|}{CPU} & \multicolumn{1}{c|}{32.5 s} & \multicolumn{1}{c|}{1.5 s} & \multicolumn{1}{c|}{4.4\%}       &  \\ \cline{1-4}
\multicolumn{1}{|c|}{GPU} & \multicolumn{1}{c|}{32.0 s} & \multicolumn{1}{c|}{1.3 s} & \multicolumn{1}{c|}{3.7\%}       &  \\ \cline{1-4}
\end{tabular}
\caption{\label{small_run} Time spent running a simulation and executing visualization per cycle. The run executed on 48 nodes with a total data set size 512 x 512 x 384. Visualization consisted of a clip, pseudocolor plot, and a volume plot.}
\end{table}

\fix{This is a small section.}