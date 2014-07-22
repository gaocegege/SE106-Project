#include "message.h"
#include <iostream>
using namespace std;

bool Message::earlierThan(Message &x)
{
	if (this->localTime.tm_year < x.localTime.tm_year)
		return true;
	else if (this->localTime.tm_year > x.localTime.tm_year)
		return false;
	else
	{
		if (this->localTime.tm_mon < x.localTime.tm_mon)
			return true;
		else if (this->localTime.tm_year > x.localTime.tm_year)
			return false;
		else
		{
			if (this->localTime.tm_mday < x.localTime.tm_mday)
				return true;
			else if (this->localTime.tm_mday > x.localTime.tm_mday)
				return false;
			else
			{
				if (this->localTime.tm_hour < x.localTime.tm_hour)
					return true;
				else if (this->localTime.tm_hour > x.localTime.tm_hour)
					return false;
				else
				{
					if (this->localTime.tm_min < x.localTime.tm_min)
						return true;
					else if (this->localTime.tm_min > x.localTime.tm_min)
						return false;
					else
					{
						if (this->localTime.tm_sec < x.localTime.tm_sec)
							return true;
						else if (this->localTime.tm_sec > x.localTime.tm_sec)
							return false;
						else
							return true;
					}
				}
			}
		}
	}
}

void Message::printMegTime()
{
	cout << this->localTime.tm_year + 1900 << "-"
		<< this->localTime.tm_mon + 1 << "-"
		<< this->localTime.tm_mday << " "
		<< this->localTime.tm_hour << ":"
		<< this->localTime.tm_min << ":"
		<< this->localTime.tm_sec << "; ";
}