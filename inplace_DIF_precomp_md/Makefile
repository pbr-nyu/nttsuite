CXXFLAGS 		+=	-g -std=c++11
C_SRCS			=	$(wildcard src/*.cpp)
C_OBJS			=	$(C_SRCS:%.cpp=%.o)
OBJS			=	$(C_OBJS)
TARGET			=	ntt_tb.out

all:$(TARGET)

$(TARGET):$(OBJS)
	$(CXX) -o $@ $(OBJS) $(LDFLAGS)

clean:
	rm -f $(TARGET) $(OBJS)
