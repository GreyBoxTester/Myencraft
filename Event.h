#pragma once
namespace cl
{
	enum class EventType
	{
		Unhandled,
		Closed,
		Resized,
		/*Minimized,
		Maximized,
		Restored,*/
		TextEntered
	};

	class Event
	{
	public:
		EventType type;
		char letter;
	};
}
