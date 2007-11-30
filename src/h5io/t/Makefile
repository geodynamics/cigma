include ../../variables

LFLAGS = -L../../lib
LIBS = $(CIGMA_LIBS) -lh5io

OPTIM += -fno-strict-aliasing

CFLAGS = $(OPTIM) $(FLAGS) $(LFLAGS)

TESTS = \
	test_h5file \
	test_h5group \
	test_h5dset \

tests: $(TESTS)

test_%: test_%.c
	$(CC) $(CFLAGS) $< -o $@ $(LIBS)

clean:
	rm -f test_io.h5
	rm -f $(TESTS)

.PHONY: clean
