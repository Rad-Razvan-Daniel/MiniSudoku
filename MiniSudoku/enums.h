#pragma once
enum class ButtonEventType {
	None,
	Pressed,
	Released,
	Moved
};
enum class Gstate {
	Intro,
	Main,
	Solving,
	Generating,
	Settings,
	Other,
	Media,
	Debug,
	SolvingAnimation,
	NumberPicker,
	IntroAnimation,
	IntroAnimationReverse
};
enum class ID {
	play,
	settings,
	media,
	generate,
	solve,
	back,
	box,
	debug,
	fastForward
};
enum buttonState {
	eNone,
	eHovered,
	eClicked
};
enum other_texture_indexes {
	eBg,
	eIntroBg,
	eBgOverlay,
	eIntroBg2,
	eIntroBg0,
	eNrPickerOverlay
};
enum interractable_texture_indexes {
	//TODO copy sfml name convention
	eBtn,
	eHover_btn,
	eActive_btn,

	eBox,
	eHover_box,
	eActive_box,

	eBack,
	eHover_back,
	eActive_back,

	eMedia,
	eHover_media,
	eActive_media,

	eSettings,
	eHover_settings,
	eActive_settings,

	eFastForward,
	eHover_ffw,
	eActive_ffw
};

struct MouseButtonEvent {
	ButtonEventType type;
	sf::Vector2i mousePos;
};