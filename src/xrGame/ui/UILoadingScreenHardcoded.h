#pragma once

pcstr GetLoadingScreenXML();
pcstr GetLoadingScreenTexturesDescr();

constexpr pcstr LoadingScreenXML =
R"(<w>
	<background width="1024" height="768">
        <auto_static width="1024" height="768">
            <texture>ui_mm_loading_screen</texture>
        </auto_static>
	</background>
    <loading_logo x="0" y="173" width="1024" height="399">
    </loading_logo>
    <loading_progress x="260" y="599" width="506" height="4" horz="1" min="0" max="100" pos="0" inertion="5.0">
        <progress>
            <texture>ui_mm_loading_progress_bar</texture>
        </progress>
        <background stretch="1">
            <texture width="506" height="4" r="0" g="0" b="0" a="255">ui_mm_loading_progress_bar</texture>
        </background>
    </loading_progress>
    <loading_stage x="260" y="530" width="506" height="20">
        <text align="c" r="170" g="170" b="170" font="letterica18"/>
    </loading_stage>
    <loading_header x="260" y="622" width="506" height="20">
        <text align="c" r="103" g="103" b="103" font="letterica18"/>
    </loading_header>
    <loading_tip_number x="260" y="658" width="506" height="20">
        <text align="c" r="103" g="103" b="103" font="letterica18"/>
    </loading_tip_number>
    <loading_tip x="163" y="676" width="700" height="80" complex_mode="1">
        <text alignment="c" align="c" r="103" g="103" b="103" font="letterica18"/>
    </loading_tip>
</w>)";

constexpr pcstr LoadingScreenXML16x9 =
R"(<w>
    <background width="1024" height="768">
        <auto_static x="102" y="0" width="819" height="768" stretch="1">
            <texture>ui_mm_loading_screen</texture>
        </auto_static>
        <auto_static x="0" y="0" width="104" height="768" stretch="1">
            <texture>ui_mm_loading_left_widepanel</texture>
        </auto_static>
        <auto_static x="920" y="0" width="104" height="768" stretch="1">
            <texture>ui_mm_loading_right_widepanel</texture>
        </auto_static>
    </background>
    <loading_logo x="102" y="173" width="819" height="512" stretch="1"/>
    <loading_progress x="310" y="599" width="405" height="5" horz="1" min="0" max="100" pos="0" inertion="5.0">
        <progress>
            <texture>ui_mm_loading_progress_bar</texture>
        </progress>
        <background stretch="1">
            <texture width="405" height="5" r="0" g="0" b="0" a="255">ui_mm_loading_progress_bar</texture>
        </background>
    </loading_progress>
    <loading_stage x="310" y="530" width="405" height="20" stretch="1">
        <text align="c" r="170" g="170" b="170" font="letterica18"/>
    </loading_stage>
    <loading_header x="310" y="622" width="405" height="20" stretch="1">
        <text align="c" r="103" g="103" b="103" font="letterica18"/>
    </loading_header>
    <loading_tip_number x="310" y="658" width="405" height="20" stretch="1">
        <text align="c" r="103" g="103" b="103" font="letterica18"/>
    </loading_tip_number>
    <loading_tip x="273" y="676" width="480" height="80" complex_mode="1" stretch="1">
        <text alignment="c" align="c" r="103" g="103" b="103" font="letterica18"/>
    </loading_tip>
</w>)";

constexpr cpcstr LoadingScreenXMLTexturesDescription =
R"(<w>
    <file name="ui\ui_actor_loadgame_screen">
        <texture id="ui_mm_loading_screen" x="0" y="0" width="1024" height="768" />
        <texture id="ui_mm_loading_progress_bar" x="0" y="772" width="506" height="4" />
    </file>
    <file name="ui\ui_actor_widescreen_sidepanels">
        <texture id="ui_mm_loading_left_widepanel" x="0" y="0" width="128" height="768" />
        <texture id="ui_mm_loading_right_widepanel" x="128" y="0" width="128" height="768" />
   </file>
</w>)";

inline pcstr GetLoadingScreenXML()
{
    return UICore::is_widescreen() ? LoadingScreenXML16x9 : LoadingScreenXML;
}

inline pcstr GetLoadingScreenTexturesDescr()
{
    return LoadingScreenXMLTexturesDescription;
}
