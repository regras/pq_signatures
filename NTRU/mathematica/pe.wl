(* ::Package:: *)

(* ::Input:: *)
(*(* Package for calculating percent error. *)*)
(*BeginPackage["pe`"]*)
(**)
(*pe::usage= *)
(*"pe[a,b] finds the percent error for two values a and b"*)
(*Begin["`Private`"]*)
(*pe[a_,b_]:=NumberForm[N[(Abs[a-b]/b)100],3]*)
(*End[]*)
(*EndPackage[]*)