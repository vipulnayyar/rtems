/*
 *  COPYRIGHT (c) 1989-2009.
 *  On-Line Applications Research Corporation (OAR).
 *
 *  The license and distribution terms for this file may be
 *  found in the file LICENSE in this distribution or at
 *  http://www.rtems.com/license/LICENSE.
 *
 *  $Id$
 */

#include "tmacros.h"
#include <assert.h>
#include <termios.h>
#include <rtems/termiostypes.h>
#include <fcntl.h>
#include <sys/errno.h>

/*
 *  Termios Test Driver
 */
#include "termios_testdriver.h"

rtems_driver_address_table test_driver = TERMIOS_TEST_DRIVER_TABLE_ENTRY;

/*
 *  Baud Rate Constant Mapping Entry
 */
typedef struct {
  int constant;
  rtems_termios_baud_t baud;
} termios_baud_test_r;

/*
 *  Baud Rate Constant Mapping Table
 */
termios_baud_test_r baud_table[] = {
  { B0,           0 },
  { B50,         50 },
  { B75,         75 },
  { B110,       110 },
  { B134,       134 },
  { B150,       150 },
  { B200,       200 },
  { B300,       300 },
  { B600,       600 },
  { B1200,     1200 },
  { B1800,     1800 },
  { B2400,     2400 },
  { B4800,     4800 },
  { B9600,     9600 },
  { B19200,   19200 },
  { B38400,   38400 },
  { B57600,   57600 },
  { B115200, 115200 },
  { B230400, 230400 },
  { B460800, 460800 },
  { -1,      -1     }
};

/*
 *  Character Size Constant Mapping Entry
 */
typedef struct {
  int constant;
  int bits;
} termios_character_size_test_r;

/*
 *  Character Size Constant Mapping Table
 */
termios_character_size_test_r char_size_table[] = {
  { CS5,      5 },
  { CS6,      6 },
  { CS7,      7 },
  { CS8,      8 },
  { -1,      -1 }
};

/*
 *  Parity Constant Mapping Entry
 */
typedef struct {
  int        constant;
  const char parity[5];
} termios_parity_test_r;

/*
 *  Parity Constant Mapping Table
 */
termios_parity_test_r parity_table[] = {
  { 0,                "none" },
  { PARENB,           "even" },
  { PARENB | PARODD,  "odd" },
  { -1,               -1 }
};

/*
 *  Stop Bit Constant Mapping Entry
 */
typedef struct {
  int   constant;
  int   stop;
} termios_stop_bits_test_r;

/*
 *  Stop Bit Constant Mapping Table
 */
termios_stop_bits_test_r stop_bits_table[] = {
  { 0,       1 },
  { CSTOPB,  2 },
  { -1,     -1 }
};

/*
 *  Test converting baud rate into an index
 */
void test_termios_baud2index(void)
{
  int i;
  int index;

  puts(
    "\n"
    "Test termios_baud2index..."
  );
  puts( "termios_baud_to_index(-2) - NOT OK" );
  i = rtems_termios_baud_to_index( -2 );
  assert ( i == -1 );

  puts( "termios_baud_to_index(572) - NOT OK" );
  i = rtems_termios_baud_to_index( -2 );
  assert ( i == -1 );

  if ( i != -1 )
  for (i=0 ; baud_table[i].constant != -1 ; i++ ) {
    printf( "termios_baud_to_index(B%d) - OK\n", baud_table[i].baud );
    index = rtems_termios_baud_to_index( baud_table[i].constant );
    if ( index != i ) {
      printf( "ERROR - returned %d should be %d\n", index, i );
      rtems_test_exit(0);
    }
  }
}

/*
 *  Test converting termios baud constant to baud number
 */
void test_termios_baud2number(void)
{
  int i;
  int number;

  puts(
    "\n"
    "Test termios_baud2number..."
  );
  puts( "termios_baud_to_number(-2) - NOT OK" );
  i = rtems_termios_baud_to_number( -2 );
  assert ( i == -1 );

  puts( "termios_baud_to_number(572) - NOT OK" );
  i = rtems_termios_baud_to_number( -2 );
  assert ( i == -1 );

  for (i=0 ; baud_table[i].constant != -1 ; i++ ) {
    printf( "termios_baud_to_number(B%d) - OK\n", baud_table[i].baud );
    number = rtems_termios_baud_to_number( baud_table[i].constant );
    if ( number != baud_table[i].baud ) {
      printf(
        "ERROR - returned %d should be %d\n",
        number,
        baud_table[i].baud
      );
      rtems_test_exit(0);
    }
  }
}

/*
 *  Test converting baud number to termios baud constant
 */
void test_termios_number_to_baud(void)
{
  int i;
  int termios_baud;

  puts(
    "\n"
    "Test termios_number_to_baud..."
  );
  puts( "termios_number_to_baud(-2) - NOT OK" );
  i = rtems_termios_number_to_baud( -2 );
  assert ( i == -1 );

  puts( "termios_number_to_baud(572) - NOT OK" );
  i = rtems_termios_number_to_baud( -2 );
  assert ( i == -1 );

  for (i=0 ; baud_table[i].constant != -1 ; i++ ) {
    printf( "termios_number_to_baud(B%d) - OK\n", baud_table[i].baud );
    termios_baud = rtems_termios_number_to_baud( baud_table[i].baud );
    if ( termios_baud != baud_table[i].constant ) {
      printf(
        "ERROR - returned %d should be %d\n",
        termios_baud,
        baud_table[i].constant
      );
      rtems_test_exit(0);
    }
  }
}

/*
 *  Test all the baud rate options
 */
void test_termios_set_baud(
  int test
)
{
  int             sc;
  int             i;
  int             number;
  struct termios  attr;

  puts( "Test termios setting device baud rate..." );
  for (i=0 ; baud_table[i].constant != -1 ; i++ ) {
    sc = tcgetattr( test, &attr );
    if ( sc != 0 ) {
      printf( "ERROR - return %d\n", sc );
      rtems_test_exit(0);
    }

    attr.c_cflag &= ~CBAUD;
    attr.c_cflag |= baud_table[i].constant;

    printf( "tcsetattr(TCSANOW, B%d) - OK\n", baud_table[i].baud );
    sc = tcsetattr( test, TCSANOW, &attr );
    if ( sc != 0 ) {
      printf( "ERROR - return %d\n", sc );
      rtems_test_exit(0);
    }

    printf( "tcsetattr(TCSADRAIN, B%d) - OK\n", baud_table[i].baud );
    sc = tcsetattr( test, TCSANOW, &attr );
    if ( sc != 0 ) {
      printf( "ERROR - return %d\n", sc );
      rtems_test_exit(0);
    }
  }
}

/*
 *  Test all the character size options
 */
void test_termios_set_charsize(
  int test
)
{
  int             sc;
  int             i;
  int             number;
  struct termios  attr;

  puts(
    "\n"
    "Test termios setting device character size ..."
  );
  for (i=0 ; char_size_table[i].constant != -1 ; i++ ) {
    sc = tcgetattr( test, &attr );
    if ( sc != 0 ) {
      printf( "ERROR - return %d\n", sc );
      rtems_test_exit(0);
    }

    attr.c_cflag &= ~CSIZE;
    attr.c_cflag |= char_size_table[i].constant;

    printf( "tcsetattr(TCSANOW, CS%d) - OK\n", char_size_table[i].bits );
    sc = tcsetattr( test, TCSANOW, &attr );
    if ( sc != 0 ) {
      printf( "ERROR - return %d\n", sc );
      rtems_test_exit(0);
    }

    printf( "tcsetattr(TCSADRAIN, CS%d) - OK\n", char_size_table[i].bits );
    sc = tcsetattr( test, TCSANOW, &attr );
    if ( sc != 0 ) {
      printf( "ERROR - return %d\n", sc );
      rtems_test_exit(0);
    }
  }
}

/*
 *  Test all the parity options
 */
void test_termios_set_parity(
  int test
)
{
  int             sc;
  int             i;
  int             number;
  struct termios  attr;

  puts(
    "\n"
    "Test termios setting device parity ..."
  );
  for (i=0 ; parity_table[i].constant != -1 ; i++ ) {
    sc = tcgetattr( test, &attr );
    if ( sc != 0 ) {
      printf( "ERROR - return %d\n", sc );
      rtems_test_exit(0);
    }

    attr.c_cflag &= ~(PARENB|PARODD);
    attr.c_cflag |= parity_table[i].constant;

    printf( "tcsetattr(TCSANOW, %s) - OK\n", parity_table[i].parity );
    sc = tcsetattr( test, TCSANOW, &attr );
    if ( sc != 0 ) {
      printf( "ERROR - return %d\n", sc );
      rtems_test_exit(0);
    }

    printf( "tcsetattr(TCSADRAIN, %s) - OK\n", parity_table[i].parity );
    sc = tcsetattr( test, TCSANOW, &attr );
    if ( sc != 0 ) {
      printf( "ERROR - return %d\n", sc );
      rtems_test_exit(0);
    }
  }
}

/*
 *  Test all the stop bit options
 */
void test_termios_set_stop_bits(
  int test
)
{
  int             sc;
  int             i;
  rtems_termios_baud_t number;
  struct termios  attr;

  puts(
    "\n"
    "Test termios setting device character size ..."
  );
  for (i=0 ; stop_bits_table[i].constant != -1 ; i++ ) {
    sc = tcgetattr( test, &attr );
    if ( sc != 0 ) {
      printf( "ERROR - return %d\n", sc );
      rtems_test_exit(0);
    }

    attr.c_cflag &= ~CSTOPB;
    attr.c_cflag |= stop_bits_table[i].constant;

    printf( "tcsetattr(TCSANOW, %d bit%s) - OK\n",
      stop_bits_table[i].stop,
      ((stop_bits_table[i].stop == 1) ? "" : "s")
    );
    sc = tcsetattr( test, TCSANOW, &attr );
    if ( sc != 0 ) {
      printf( "ERROR - return %d\n", sc );
      rtems_test_exit(0);
    }

    printf( "tcsetattr(TCSADRAIN, %d bits) - OK\n", stop_bits_table[i].stop );
    sc = tcsetattr( test, TCSANOW, &attr );
    if ( sc != 0 ) {
      printf( "ERROR - return %d\n", sc );
      rtems_test_exit(0);
    }
  }
}

rtems_task Init(
  rtems_task_argument ignored
)
{
  int                       rc;
  rtems_status_code         sc;
  rtems_device_major_number registered;
  int                       test;

  puts( "\n\n*** TEST TERMIOS 01 ***" );

  test_termios_baud2index();
  test_termios_baud2number();
  test_termios_number_to_baud();

  puts(
    "\n"
    "Init - rtems_io_register_driver - Termios Test Driver - OK"
  );
  sc = rtems_io_register_driver( 0, &test_driver, &registered );
  printf( "Init - Major slot returned = %d\n", (int) registered );
  directive_failed( sc, "rtems_io_register_driver" );

  /*
   * Test baud rate
   */
  puts( "Init - open - " TERMIOS_TEST_DRIVER_DEVICE_NAME " - OK" );
  test = open( TERMIOS_TEST_DRIVER_DEVICE_NAME, O_RDWR );
  if ( test == -1 ) {
    printf( "ERROR - baud opening test device (%d)\n", test );
    rtems_test_exit(0);
  }

  test_termios_set_baud(test);

  puts( "Init - close - " TERMIOS_TEST_DRIVER_DEVICE_NAME " - OK" );
  rc = close( test );
  if ( rc != 0 ) {
    printf( "ERROR - baud close test device (%d) %s\n", test, strerror(errno) );
    rtems_test_exit(0);
  }

  /*
   * Test character size
   */ 
  puts(
    "\n"
    "Init - open - " TERMIOS_TEST_DRIVER_DEVICE_NAME " - OK"
  );
  test = open( TERMIOS_TEST_DRIVER_DEVICE_NAME, O_RDWR );
  if ( test == -1 ) {
    printf( "ERROR - size open test device (%d) %s\n", test, strerror(errno) );
    rtems_test_exit(0);
  }

  test_termios_set_charsize(test);

  puts( "Init - close - " TERMIOS_TEST_DRIVER_DEVICE_NAME " - OK" );
  rc = close( test );
  if ( rc != 0 ) {
    printf( "ERROR - size close test device (%d) %s\n", test, strerror(errno) );
    rtems_test_exit(0);
  }

  /*
   * Test parity
   */ 
  puts(
    "\n"
    "Init - open - " TERMIOS_TEST_DRIVER_DEVICE_NAME " - OK"
  );
  test = open( TERMIOS_TEST_DRIVER_DEVICE_NAME, O_RDWR );
  if ( test == -1 ) {
    printf( "ERROR - parity open test device (%d) %s\n",
      test, strerror(errno) );
    rtems_test_exit(0);
  }

  test_termios_set_parity(test);

  puts( "Init - close - " TERMIOS_TEST_DRIVER_DEVICE_NAME " - OK" );
  rc = close( test );
  if ( rc != 0 ) {
    printf( "ERROR - parity close test device (%d) %s\n",
      test, strerror(errno) );
    rtems_test_exit(0);
  }

  /*
   * Test stop bits
   */ 
  puts(
    "\n"
    "Init - open - " TERMIOS_TEST_DRIVER_DEVICE_NAME " - OK"
  );
  test = open( TERMIOS_TEST_DRIVER_DEVICE_NAME, O_RDWR );
  if ( test == -1 ) {
    printf( "ERROR - stop bits open test device (%d) %s\n",
      test, strerror(errno) );
    rtems_test_exit(0);
  }

  test_termios_set_stop_bits(test);

  puts( "Init - close - " TERMIOS_TEST_DRIVER_DEVICE_NAME " - OK" );
  rc = close( test );
  if ( rc != 0 ) {
    printf( "ERROR - stop bits close test device (%d) %s\n",
      test, strerror(errno) );
    rtems_test_exit(0);
  }

  puts( "*** END OF TEST TERMIOS 01 ***" );
  rtems_test_exit(0);
}

/* configuration information */

#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER

/* include an extra slot for registering the termios one dynamically */
#define CONFIGURE_MAXIMUM_DRIVERS 3

/* one for the console and one for the test port */
#define CONFIGURE_NUMBER_OF_TERMIOS_PORTS 2

/* we need to be able to open the test device */
#define CONFIGURE_LIBIO_MAXIMUM_FILE_DESCRIPTORS 4

#define CONFIGURE_MAXIMUM_TASKS         1
#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_INIT
#include <rtems/confdefs.h>

/* global variables */