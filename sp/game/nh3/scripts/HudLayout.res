"Resource/HudLayout.res"
{
  HudHealthBar
  {

  	"fieldName" "HudHealthBar"
  	"visible" "1"
  	"enabled" "1"
  	"xpos" "15"
  	"ypos" "440"
  	"wide" "124"
  	"tall" "35"
  	"BarWidth" "79"
  	"BarHeight" "19"
  	"BarInsetX" "40"
  	"BarInsetY" "8"
  	"TextFont" "HUD_NH2_Health"
	"My_Alpha"          "255"
  	"text_xpos" "6"
  	"text_ypos" "2"
  	"TextColor" "206 206 206 255"
  	"PaintBackgroundType" "1"
  }

  HudVeins
  {

  	"fieldName" "HudVeins"
  	"visible" "1"
  	"enabled" "1"
  	"xpos" "0"
  	"ypos" "0"
  	"wide" "640"
  	"tall" "480"
  }
  
  HudTorch
  {
  	"fieldName" "HudTorch"
  	"visible" "1"
  	"enabled" "1"
  	"xpos" "140"
  	"ypos" "455"
  	"wide" "75"
  	"tall" "18"
  	"BarWidth" "40"
  	"BarHeight" "10"
  	"BarInsetX" "30"
  	"BarInsetY" "5"
  	"icon_xpos" "6"
  	"icon_ypos" "5"
  	"icon_wide" "20"
  	"icon_tall" "10"
	"TextColor"	"255 255 255 255"
  	"PaintBackgroundType" "1"
  }
  HudStamina
  {
  	"fieldName" "HudStamina"
  	"visible" "1"
  	"enabled" "1"
  	"xpos" "215"
  	"ypos" "455"
  	"wide" "74"
  	"tall" "18"
  	"BarWidth" "47"
  	"BarHeight" "10"
  	"BarInsetX" "22"
  	"BarInsetY" "5"
  	"icon_xpos" "6"
  	"icon_ypos" "3"
  	"icon_wide" "12"
  	"icon_tall" "13"
  	"PaintBackgroundType" "1"
	"TextColor"	"255 255 255 255"
  }
  
  HudWeaponSelection
	{
		"fieldName" "HudWeaponSelection"
		"visible" "1"
		"enabled" "1"
		"SmallBoxSize" "32"
		"MediumBoxWide"	"95"
		"MediumBoxWide_hidef"	"78"
		"MediumBoxTall"	"50"
		"MediumBoxTall_hidef"	"50"
		"MediumBoxWide_lodef"	"74"
		"MediumBoxTall_lodef"	"50"
		"LargeBoxWide" "112"
		"LargeBoxTall" "80"
		"BoxGap" "8"
		"SelectionNumberXPos" "4"
		"SelectionNumberYPos" "4"
		"SelectionGrowTime"	"0.4"
		"TextYPos" "64"
		"TextYPos" "64"
		"WepXPos" "11"
		"WepYPos" "18"
		"WepXSize" "90"
		"WepYSize" "40"
	}
  
  HudSquad
  {
    "fieldname" "HudSquad"
    "visible" "0"
  	"enabled" "1"
  	"xpos" "675"
  	"ypos" "445"
  	"iconwide" "16"
  	"icontall" "18"
  	"wide" "81"
  	"tall" "27"
  	"IconFont" "HudNumbers"
  	"IconInsetX" "6"
  	"IconInsetY" "4"
  	"IconGap"  "18"
  	"SquadIconColor" "255 255 255 160"
  	
  }
  
  
  HudCrosshair
	{
		"fieldName" "HudCrosshair"
		"visible" "1"
		"enabled" "1"
		"wide"	 "640"
		"tall"	 "480"
	}

	HudDeathNotice
	{
		"fieldName" "HudDeathNotice"
		"visible" "1"
		"enabled" "1"
		"wide"	 "640"
		"tall"	 "480"
	}
	
	
	HudMessage
	{
		"fieldName" "HudMessage"
		"visible" "1"
		"enabled" "1"
		"wide"	 "640"
		"tall"	 "480"
	}

	HudMenu
	{
		"fieldName" "HudMenu"
		"visible" "0"
		"enabled" "0"
		"wide"	 "640"
		"tall"	 "480"
	}

	HudCloseCaption
	{
		"fieldName" "HudCloseCaption"
		"visible"	"0"
		"enabled"	"1"
		"xpos"		"c-250"
		"ypos"		"276"	[$WIN32]
		"ypos_hidef"	"236"	[$X360]
		"ypos_lodef"	"206"	[$X360]			//236
		"wide"		"500"
		"tall"		"136"	[$WIN32]
		"tall"		"176"	[$X360]

		"BgAlpha"	"128"

		"GrowTime"		"0.25"
		"ItemHiddenTime"	"0.2"  // Nearly same as grow time so that the item doesn't start to show until growth is finished
		"ItemFadeInTime"	"0.15"	// Once ItemHiddenTime is finished, takes this much longer to fade in
		"ItemFadeOutTime"	"0.3"
		"topoffset"		"0"		[$WIN32]
		"topoffset"		"40"	[$X360]
	}
	
	HudHintDisplay
	{
		"fieldName"	"HudHintDisplay"
		"visible"	"0"
		"enabled" "1"
		"Alpha"		"0"		// Remove this to enable hint hud element
		"xpos"	"r120"	[$WIN32]
		"ypos"	"r340"	[$WIN32]
		"xpos"	"r148"	[$X360]
		"ypos"	"r338"	[$X360]
		"wide"	"100"
		"tall"	"700"
		"text_xpos"	"8"
		"text_ypos"	"8"
		"text_xgap"	"8"
		"text_ygap"	"8"
		"TextColor"	"255 255 255 220"

		"PaintBackgroundType"	"2"
	}

	HudHintKeyDisplay
	{
		"fieldName"	"HudHintKeyDisplay"
		"visible"	"0"
		"enabled" 	"1"
		"xpos"		"r120"	[$WIN32]
		"ypos"		"r340"	[$WIN32]
		"xpos"		"r148"	[$X360]
		"ypos"		"r338"	[$X360]
		"wide"		"100"
		"tall"		"200"
		"text_xpos"	"8"
		"text_ypos"	"8"
		"text_xgap"	"8"
		"text_ygap"	"8"
		"TextColor"	"255 255 255 220"

		"PaintBackgroundType"	"2"
	}
	
	HudCommentary
	{
		"fieldName" "HudCommentary"
		"xpos"	"c-190"
		"ypos"	"350"
		"wide"	"380"
		"tall"  "40"
		"visible" "1"
		"enabled" "1"
		
		"PaintBackgroundType"	"2"
		
		"bar_xpos"		"50"
		"bar_ypos"		"20"
		"bar_height"	"8"
		"bar_width"		"320"
		"speaker_xpos"	"50"
		"speaker_ypos"	"8"
		"count_xpos_from_right"	"10"	// Counts from the right side
		"count_ypos"	"8"
		
		"icon_texture"	"vgui/hud/icon_commentary"
		"icon_xpos"		"0"
		"icon_ypos"		"0"		
		"icon_width"	"40"
		"icon_height"	"40"
	}
	
	HudCredits
	{
		"fieldName"	"HudCredits"
		"TextFont"	"Default"
		"visible"	"1"
		"xpos"	"0"
		"ypos"	"0"
		"wide"	"640"
		"tall"	"480"
		"TextColor"	"255 255 255 192"

	}
	
	HudSuitPower
	{
		"fieldName" "HudSuitPower"
		"visible" "0"
		"enabled" "0"
		"wide"	"0"
		"tall"	"0"
	}
	
	HudAmmoNH2
	{
	 "fieldName" "HudAmmoNH2"
	 "visible" "1"
	 "enabled" "1"
	 "xpos"	"16"
	 "ypos"	"407"
	 "icon_xpos" "84"
	 "icon_ypos" "-16"
	 "line_xpos" "42"
	"line_ypos" "6"
	"line_wide" "2"
	"line_tall" "20"
	"line_color" "206 206 206 255"
	"TextFont" "HUD_NH2_Health"
	"TextColor" "206 206 206 255"
	"ammo1_xpos" "13"
	"ammo1_ypos" "1"
	"ammo2_xpos" "50"
	"ammo2_ypos" "1"
	"wide" "125"
	"tall" "33"
  }
  
  HudHistoryResource
	{
		"fieldName" "HudHistoryResource"
		"visible" "1"
		"enabled" "1"
		"xpos"	"r252"
		"ypos"	"40"
		"wide"	 "248"
		"tall"	 "320"

		"history_gap"	"56"
		"icon_inset"	"38"
		"text_inset"	"36"
		"NumberFont"	"HudNumbersSmall"
	}
	
	

}
