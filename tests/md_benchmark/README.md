<!--
	SPDX-FileCopyrightText: 2022-2024 Igor Mironchik <igor.mironchik@gmail.com>
	SPDX-License-Identifier: MIT
-->

# Why `md4qt` is slower `cmark-gfm`?

Guys, this is a fight of `QString` and `char *` :)

## C/C++/Qt

```cpp
#include <chrono>
#include <iostream>
#include <string>
#include <QString>


int main( int argc, char ** argv )
{
    {
        const auto start = std::chrono::high_resolution_clock::now();

        {
            const QString s = QStringLiteral(
                "abcdabcdabcdabcdabcdabcdabcdabcd"
                "abcdabcdabcdabcdabcdabcdabcdabcdabcdabcd" );
            QString t;

            for( auto i = 0; i < s.size(); ++i )
            {
                if( s[ i ] == QLatin1Char( 'a' ) )
                    t.append( s[ i ] );
            }
        }

        const auto end = std::chrono::high_resolution_clock::now();

        const auto d = std::chrono::duration_cast< std::chrono::nanoseconds > (
            end - start );

        std::cout << "QString: " << d.count() << " ns" << std::endl;
    }

    {
        const auto start = std::chrono::high_resolution_clock::now();

        {
            const std::string s( "abcdabcdabcdabcdabcdabcdabcd"
                "abcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcd" );
            std::string t;

            for( auto i = 0; i < s.size(); ++i )
            {
                if( s[ i ] == 'a' )
                    t.push_back( s[ i ] );
            }
        }

        const auto end = std::chrono::high_resolution_clock::now();

        const auto d = std::chrono::duration_cast< std::chrono::nanoseconds > (
            end - start );

        std::cout << "std::string: " << d.count() << " ns" << std::endl;
    }

    {
        const auto start = std::chrono::high_resolution_clock::now();

        {
            const char * s = "abcdabcdabcdabcdabcdabcdabcd"
                "abcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcd";
            const auto len = strlen( s );
            char * t = (char*) malloc( len / 4 + 1 );

            auto j = 0;

            for( auto i = 0; i < len; ++i )
            {
                if( s[ i ] == 'a' )
                    t[ j++ ] = s[ i ];
            }

            t[ j ] = '\0';

            free( t );
        }

        const auto end = std::chrono::high_resolution_clock::now();

        const auto d = std::chrono::duration_cast< std::chrono::nanoseconds > (
            end - start );

        std::cout << "char*: " << d.count() << " ns" << std::endl;
    }

    return 0;
}
```

### Results

```
QString: 11354 ns
std::string: 7118 ns
char*: 398 ns
```

## And Java for fun :)

```java
public class JavaProgram {

   public static void main(String []args) {
      long start = System.nanoTime();
      
      String s = "abcdabcdabcdabcdabcdabcdabcdabcdabcd" +
          "abcdabcdabcdabcdabcdabcdabcdabcdabcd";
      String t = "";
      
      for(int i = 0; i < s.length(); ++i)
      {
         if(s.charAt(i) == 'a')
            t += s.charAt(i);
      }
      
      long end = System.nanoTime();
            
      System.out.println("Java: " + (end - start) + " ns.");
   }
}
```

### Result

```
$ java JavaProgram 
Java: 37983904 ns.
$ java JavaProgram 
Java: 45452295 ns.
$ java JavaProgram 
Java: 45725337 ns.
$ java JavaProgram 
Java: 32287526 ns.
```
