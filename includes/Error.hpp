#ifndef Error_HPP
# define Error_HPP

//Static backend handler
class Error 
{
	public:
		static int message(std::string error)
		{
			std::cerr << error << std::endl;
			return (0);
		}
};

#endif