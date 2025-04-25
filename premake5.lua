workspace "Arcane"
	configurations { "Debug", "Release" }
	startproject "Game"

	include "Engine/Libraries/ImGui"
	include "Engine"
	include "Game"