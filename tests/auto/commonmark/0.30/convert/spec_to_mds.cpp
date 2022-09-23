
#include <fstream>
#include <string>
#include <iostream>

std::string convert_escape_ch( const std::string & s )
{
	std::string res;
	bool escape = false;

	for( const auto c : s )
	{
		if( c == '\\' && !escape )
			escape = true;
		else
		{
			if( escape )
			{
				switch( c )
				{
					case 't' :
						res += '\t';
						break;

					case 'n' :
						res += '\n';
						break;

					case '\\' :
						res += '\\';
						break;

					case '"' :
						res += '"';
						break;

					default :
						throw std::logic_error( std::string( "Unknown escape character: \"" ) +
							c + "\"" );
				}

				escape = false;
			}
			else
				res += c;
		}
	}

	return res;
}


int main()
{
	try {
		std::ifstream in( "spec.json" );

		size_t mc = 1;

		if( in.good() )
		{
			for( std::string line; std::getline( in, line ); )
			{
				auto mp = line.find( "markdown" );

				if( mp != std::string::npos )
				{
					mp += 9;

					for( ; mp < line.size(); ++mp )
					{
						if( line[ mp ] == '"' )
							break;
					}

					if( mp + 1 < line.size() )
					{
						const auto ms = convert_escape_ch( line.substr( mp + 1, line.size() - mp - 3 ) );

						auto fn = std::to_string( mc );

						if( fn.size() < 3 )
							fn = std::string( 3 - fn.size(), '0' ) + fn;

						fn += ".md";

						++mc;

						std::ofstream f( fn );

						if( f.good() )
							f.write( ms.c_str(), ms.size() );
						else
						{
							std::cout << "Unable to create file \"" << fn << "\n" << std::endl;

							return -1;
						}
					}
				}
			}
		}
		else
		{
			std::cout << "Unable to open file \"spec.json\"" << std::endl;

			return -1;
		}
	}
	catch( const std::logic_error & x )
	{
		std::cout << x.what() << std::endl;

		return -1;
	}
}
