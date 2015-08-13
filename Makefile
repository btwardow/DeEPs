CXXFLAGS =	-O2 -g -Wall -fmessage-length=0

OBJS =		DeEPs.o Attribute.o DataSet.o Log.o Instance.o ReducedDataSet.o Pattern.o LazyClassifier.o DeEPsClassifier.o

LIBS =

TARGET =	DeEPs

$(TARGET):	$(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

all:	$(TARGET)

clean:
	rm -f $(OBJS) $(TARGET)
