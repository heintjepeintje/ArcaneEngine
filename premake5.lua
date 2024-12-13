workspace "Arcane"
	configurations { "Debug", "Release" }
	startproject "Game"

	include "Engine/Libraries/glad"
	include "Engine"
	include "Game"