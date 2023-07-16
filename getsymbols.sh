cat *.c++ *.h++|sed 's_//.*__'|sed 's/"[^"]*"//'|grep -oh '[a-zA-Z_0-9:~]\+'|sort|uniq|grep -v '^:'|grep -v '_DEBUG$'|grep -v '_H$'
