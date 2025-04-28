workspace "Arcane"
	configurations { "Debug", "Release" }
	startproject "Game"

	include "Engine/Libraries/tracy"
	include "Engine"
	include "Game"