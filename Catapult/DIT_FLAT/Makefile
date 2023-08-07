CXXFLAGS 		+=	-g -I$(MGC_HOME)shared/include -std=c++11
C_SRCS			=	$(wildcard src/*.cpp)
C_OBJS			=	$(C_SRCS:%.cpp=%.o)
OBJS			=	$(C_OBJS)
TARGET			=	ntt_tb.out
CXX_CATAPULT 	= 	$(MGC_HOME)/bin/g++

all:$(TARGET)

$(TARGET):$(OBJS)
	$(CXX_CATAPULT) -o $@ $(OBJS) $(LDFLAGS)

clean:
	rm -f $(TARGET) $(OBJS)
