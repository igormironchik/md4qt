# Conclusion

Guys, this is a fight of `QString` and `char *` :)

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
                "abcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcd" );
            QString t;

            for( auto i = 0; i < s.size(); ++i )
            {
                if( s[ i ] == QLatin1Char( 'a' ) )
                    t.append( s[ i ] );
            }
        }

        const auto end = std::chrono::high_resolution_clock::now();

        const auto d = std::chrono::duration_cast< std::chrono::nanoseconds > ( end - start );

        std::cout << "QString: " << d.count() << " ns" << std::endl;
    }

    {
        const auto start = std::chrono::high_resolution_clock::now();

        {
            const std::string s( "abcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcd" );
            std::string t;

            for( auto i = 0; i < s.size(); ++i )
            {
                if( s[ i ] == 'a' )
                    t.push_back( s[ i ] );
            }
        }

        const auto end = std::chrono::high_resolution_clock::now();

        const auto d = std::chrono::duration_cast< std::chrono::nanoseconds > ( end - start );

        std::cout << "std::string: " << d.count() << " ns" << std::endl;
    }

    {
        const auto start = std::chrono::high_resolution_clock::now();

        {
            const char * s = "abcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcd";
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

        const auto d = std::chrono::duration_cast< std::chrono::nanoseconds > ( end - start );

        std::cout << "char*: " << d.count() << " ns" << std::endl;
    }

    return 0;
}
```

## Results

```
QString: 14211 ns
std::string: 4039 ns
char*: 317 ns
```

## And Java for fun :)

```java
public class JavaProgram {

   public static void main(String []args) {
      long start = System.nanoTime();
      
      String s = "abcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcdabcd";
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
Java: 108094986 ns.
$ java JavaProgram 
Java: 74303385 ns.
$ java JavaProgram 
Java: 85103451 ns.
$ java JavaProgram 
Java: 96809767 ns.
```

# Why don't I use `md4c` for Markdown parsing?

`md4c` completely wrong with complex footnotes, that is needed for `md-pdf` render.
Ok, I don't support `LaTeX` math expressions yet. But this is because I don't
need it yet. If you need something that is not implemented yet - just create an issue.
