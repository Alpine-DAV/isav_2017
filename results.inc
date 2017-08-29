\section{Results}
\label{sec:results}
We evaluated the performance of the ALPINE infrastructure using the included Kripke physics proxy-application that models deterministic neutron transport. 
%
Running on 48 nodes equipped with 16-core Intel Xeon CPUs and NVIDIA K40m GPUs, we ran Kripke with a total problem size of 100M zones.
%
Each cycle, we clipped the mesh in half, created a pseudocolor plot of the clipped mesh, and combined the result with a volume rendering of the mesh. 
%
The average time per cycle spent performing simulation and visualization is shown in Table~\ref{small_run}, and the resulting image is shown in Figure~\ref{kripke}.
%
Visualization results in less than 5\% of total runtime.



\begin{table}[]
\centering
\begin{tabular}{ccccl}
\cline{1-4}
\multicolumn{1}{|c|}{}    & \multicolumn{1}{c|}{Sim}  & \multicolumn{1}{c|}{Vis} & \multicolumn{1}{c|}{Percentage Vis} &  \\ \cline{1-4}
\multicolumn{1}{|c|}{CPU} & \multicolumn{1}{c|}{32.5 s} & \multicolumn{1}{c|}{1.5 s} & \multicolumn{1}{c|}{4.4\%}       &  \\ \cline{1-4}
\multicolumn{1}{|c|}{GPU} & \multicolumn{1}{c|}{32.0 s} & \multicolumn{1}{c|}{1.3 s} & \multicolumn{1}{c|}{3.7\%}       &  \\ \cline{1-4}
\end{tabular}
\caption{\label{small_run} Average time in seconds spent running a simulation and executing visualization per cycle as described in Section~\ref{sec:results}.}
\end{table}
