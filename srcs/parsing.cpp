#include "client.hpp"
#include "server.hpp"

vectorCommand	ParsCommand(std::string const &msg)
{
	vectorCommand		VectorArgs;
	size_t				FindDoublePoint = msg.find(":");

	if (FindDoublePoint != msg.npos)
	{
		std::string	Msg;
		std::string	Args;
		std::string	PushBackArg;

		Args = msg.substr(0, FindDoublePoint);
		Msg = msg.substr(FindDoublePoint, msg.size());
		std::stringstream	SepMsg(Args);
		while (!SepMsg.eof())
		{
			SepMsg >> PushBackArg;
			if (PushBackArg != "" && PushBackArg != "\r\n")
				VectorArgs.push_back(PushBackArg);
			PushBackArg.clear();
		}
		VectorArgs.push_back(Msg);
		return (VectorArgs);
	}
	std::string	PushBackArg;
	std::stringstream	SepMsg(msg);
	while (!SepMsg.eof())
	{
		SepMsg >> PushBackArg;
		VectorArgs.push_back(PushBackArg);
		PushBackArg.clear();
	}
	return (VectorArgs);
}
