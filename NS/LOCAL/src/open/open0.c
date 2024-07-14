#include    <sv_base.h> /*replacement macros */
#include    <sys/types.h>
#include    <errno.h>
#include    <stdio.h>
#include    <sys/stat.h>
#include    <string.h>
#include    <fcntl.h>
/*    #include    <limits.h>    */
#include    <svvs.h>
#include    <sv_env.h>
#include    <sv_macro.h>

#ident    "@(#)open0.c	1.11"

# define NOERR		0
# define FILENAME    "open.data"
# define BAD_PATH    "badpath.data/"
# define MISSING_DIR    "missing_dir/"
# define NO_SEARCH_DIR    "nosearch.dir/"
# define NO_WR_DIR    "nowrite.dir/"
# define DIR777        "open.dir/"
# define NO_READ_FILE    "noread.data"
# define NO_WR_FILE    "nowrite.data"

#undef	strcat	/*  disable error checking  */
#undef	strlen	/*  disable error checking  */
#undef	strcpy	/*  disable error checking  */
#undef	strncmp	/*  disable error checking  */

int oflgs[] = {
	O_RDONLY, O_WRONLY, O_RDWR,
#ifndef	SYSVR_2		/*  O_SYNC is for SVR3 and later  */
	O_RDONLY | O_SYNC, O_WRONLY | O_SYNC, O_RDWR | O_SYNC,
#endif
	};

int perms[] = {
    0666, 0444, 0222 };

main()
{ /* begin main */
    int        i, n, o, a, c, t, x;
    int        bits, oflag, new, err;
    int        ret_val;
    int        r_val[OPEN_MAX];
    char        path[PATH_MAX + 10];

    char        *data_path();

    setup( "open" );

    funct("open");

# ifndef SYSVR_2
	map_modes(1);	/* do SVR2, SVR3 mode mapping */
# endif

    exit_on ( ERROR );
    start

        strcpy( path, data_path() );
        strcat( path, FILENAME );

        for (n=0;n<4;n++)
        { /* begin for(n) */
            if(!n) new=0;
            else 
            { /* begin else */
                new = 1;
                bits = perms[n-1];
            } /* end else */
#ifdef	SYSVR_2
		for (o=0;o<3;o++)
#else
		for (o=0;o<6;o++)	/*  test for O_SYNC  */
#endif
            { /* begin for(o) */
                oflag = oflgs[o];
                for (a=0;a<2;a++)
                { /* begin for(a) */
                    if (a) oflag |= O_APPEND;
                    else oflag &= ~O_APPEND;
                    for (c=0;c<2;c++)
                    { /* begin for(c) */
                        if (c) oflag |= O_CREAT;
                        else oflag &= ~O_CREAT;
                        for (t=0;t<2;t++)
                        { /* begin for(t) */
                            if (t) oflag |= O_TRUNC;
                            else oflag &= ~O_TRUNC;
                            for (x=0;x<2;x++)
                            { /* begin for(x) */
                                if (x) oflag |= O_EXCL;
                                else oflag &= ~O_EXCL;

                                switch (n)
                                { /* begin switch(n) */
                                case 0:
                                    testing("Open of new file");
                                    break;
                                case 1:
                                    testing("Open of existing file with read/write permission");
                                    break;
                                case 2:
                                    testing("Open of existing file with read only permission");
                                    break;
                                case 3:
                                    testing("Open of existing file with write only permission");
                                    break;
                                } /* end switch(n) */
                                switch (o)
                                { /* begin switch(o) */

				case 0:
					testing("for O_RDONLY");
					break;
				case 1:
					testing("for O_WRONLY");
					break;
				case 2:
					testing("for O_RDWR");
					break;
				case 3:
					testing("for O_RDONLY | O_SYNC");
					break;
				case 4:
					testing("for O_WRONLY | O_SYNC");
					break;
				case 5:
					testing("for O_RDWR | O_SYNC");
					break;
                                } /* end switch(o) */
                                if (a)
                                { /* begin if(a) */
                                    testing("| O_APPEND");
                                } /* end if(a) */
                                if (c)
                                { /* begin if(c) */
                                    testing("| O_CREAT");
                                } /* end if(c) */
                                if (t)
                                { /* begin if(t) */
                                    testing("| O_TRUNC");
                                } /* end if(t) */
                                if (x)
                                { /* begin if(x) */
                                    testing("| O_EXCL");
                                } /* end if(x) */
                                expecting ( SUCCESS );
				err=NOERR;
                                if (new && c && x)
                                { /* begin if(new && c && x) */
                                    testing("Expect a return of (-1), errno of EEXIST, unchanged file.");
                                    err=EEXIST;
                                } /* end if (new && c && x) */
                                else if (!new && !c)
                                { /* begin if(!new && !c) */
                                    testing("Expect a return of (-1), errno of ENOENT.");
                                    err=ENOENT;
                                } /* end if (!new && !c) */
                                else if (n==2 && ((o%3) || t) || n==3 && (o%3)!=1 )
                                { /* begin if(n==2 ...) */
                                    testing("Expect a return of (-1), errno of EACCES.");
                                    err=EACCES;
                                } /* end if (n==2 && o || n == 3 && o != 1) */
                                else 
                                {
                                    if (c)
                                    {
                                        testing("non-negative return.");
                                    }
                                    else if (n==3)
                                    {
                                        testing("non-negative return and unchanged file status.");
                                    }
                                    else 
                                    {
                                        testing("non-negative return, unchanged file status and contents.");
                                    }
                                }
                                expecting ( VOID );
                                (void)unlink(path);
				start
                                test_case( path, oflag, 0666, new, bits, err );
				finis
                            } /* end for(x) */
                        } /* end for(t) */
                    } /* end for(c) */
                } /* end for(a) */
            } /* end for(o) */
        } /* end for(n) */
    finis

    testing("Open of path with a non-directory component.");
    testing("Expect a return of (-1) and errno of ENOTDIR.");
    start

        strcpy( path, data_path() );
        strcat( path, BAD_PATH );
        strcat( path, FILENAME );

        expecting ( ENOTDIR );
        open( path, O_RDONLY );

    finis

    testing("Open of path with a missing directory component.");
    testing("Expect a return of (-1) and errno of ENOENT.");
    start


        strcpy( path, data_path() );
        strcat( path, MISSING_DIR );
        strcat( path, FILENAME );

        expecting ( ENOENT );
        open( path, O_WRONLY | O_CREAT, 0666 );

    finis


    testing("Open of path with a component which denies search permission.");
    testing("Expect a return of (-1) and errno of EACCES.");
    start

        strcpy( path, data_path() );
        strcat( path, NO_SEARCH_DIR );
        strcat( path, FILENAME );

        expecting ( EACCES );
        open( path, O_RDONLY );


    finis

    testing("Open of new file in path with a component which denies");
    testing("write permission.");
    testing("Expect a return of (-1) and errno of EACCES.");
    start

        strcpy( path, data_path() );
        strcat( path, NO_WR_DIR );
        strcat( path, FILENAME );

        expecting ( EACCES );
        open( path, O_WRONLY | O_CREAT, 0666 );


        strcpy( path, data_path() );
        strcat( path, DIR777 );

#ifdef	SYSVR_2
        for (o=0;o<3;o++)
#else
	for (o=0;o<6;o++)  /*  test for O_SYNC  */
#endif

        { /* begin for(o) */
            oflag = oflgs[o];
            for (a=0;a<2;a++)
            { /* begin for(a) */
                if (a) oflag |= O_APPEND;
                else oflag &= ~O_APPEND; /* this line for good style */
                for (c=0;c<2;c++)
                { /* begin for(c) */
                    if (c) oflag |= O_CREAT;
                    else oflag &= ~O_CREAT;
                    for (t=0;t<2;t++)
                    { /* begin for(t) */
                        if (t) oflag |= O_TRUNC;
                        else oflag &= ~O_TRUNC;
                        for (x=0;x<2;x++)
                        { /* begin for(x) */
                            if (x) oflag |= O_EXCL;
                            else oflag &= ~O_EXCL;

                            testing("Open of a directory");
                            switch (o)
                            { /* begin switch(o) */

                            case 0:
                                testing("for O_RDONLY");
                                break;
                            case 1:
                                testing("for O_WRONLY");
                                break;
                            case 2:
                                testing("for O_RDWR");
                                break;
                            case 3:
                                testing("for O_RDONLY | O_SYNC");
                                break;
                            case 4:
                                testing("for O_WRONLY | O_SYNC");
                                break;
                            case 5:
                                testing("for O_RDWR | O_SYNC");
                                break;
                            } /* end switch(o) */
                            if (a)
                            { /* begin if(a) */
                                testing("| O_APPEND");
                            } /* end if(a) */
                            if (c)
                            { /* begin if(c) */
                                testing("| O_CREAT");
                            } /* end if(c) */
                            if (t)
                            { /* begin if(t) */
                                testing("| O_TRUNC");
                            } /* end if(t) */
                            if (x)
                            { /* begin if(x) */
                                testing("| O_EXCL");
                            } /* end if(x) */
                            err=NOERR;
                            if (c && x)
                            { /* begin if(new && c && x) */
                                testing("Expect a return of (-1), errno of EEXIST, unchanged file.");
                            err=EEXIST;
                            } /* end if (new && c && x) */
                            else if ((o%3) || t)
                            { /* begin if((o%3)||t) */
                                testing("Expect a return of (-1), errno of EISDIR.");
                            err=EISDIR;
                            } /* end if (o || t) */
                            else 
                            {
                            err=NOERR;
                                testing("Non-negative return, unchanged file status and contents.");
                            }
			   start
                            test_case( path, oflag, 01777, 0, bits, err );
			   finis
                        } /* end for(x) */
                    } /* end for(t) */
                } /* end for(c) */
            } /* end for(a) */
        } /* end for(o) */

    finis



    testing("Open of OPEN_MAX+1 files for O_WRONLY.");
    testing("Expect the lowest available file descriptor  for first OPEN_MAX files.");
    testing("Expect a return of (-1) and errno of EMFILE for last file.");
    start

        strcpy( path, data_path() );
        strcat( path, DIR777 );
        strcat( path, FILENAME );


	prep("Consume all available file descriptors.");
	start
        for( i = 0; i < (OPEN_MAX-3) ; i++ ) /* 0, 1, 2 taken */
        {
            expecting ( SUCCESS );
            r_val[i] = open( path, O_WRONLY | O_CREAT, 0666 );


        }
	finis

	testing("Verify the file descriptors.");
	start

        for( i = 0; i < (OPEN_MAX-3) ; i++ )
        {
            if( r_val[i] != i+3 )
            {
                error("Open did not return lowest available file descriptor.\nThe expected descriptor value was %d.\nThe actual descriptor value was %d.", r_val[i], (i+3));    /* ERROR */
                break;
            }
        }

	finis

	testing("The next open should generate an error.");
	start
        expecting ( EMFILE );
        ret_val = open( path, O_WRONLY | O_CREAT, 0666 );
	finis

	cleanup("Release all open file descriptors.");
	start
        expecting ( SUCCESS );
        for( i = 0; i < (OPEN_MAX-3) ; i++ ) /* 0, 1, 2 taken */
        {
            expecting ( VOID );
            if( close( r_val[i] ) != 0 )
                severr("close failed");
        }
	finis

    finis

    done();
    /*NOTREACHED*/
} /* end main */

test_case( path, oflag, mode, new, bits, err)
char *path;
int oflag;
int mode;
int new;
int bits;
int err;
{
    int    sts;
    ttime    t;
    int    stflg;
    static    struct stat    st_buf1;
    static    struct stat    st_buf2;
    static    char    buf1[FILESIZ];
    static    char    buf2[FILESIZ];
    static    int    init_flg = 1;

    ttime    time();
    void    cmp_stat();
    void    init_buf();
    void    get_file();
    void    put_file();
    char    *err_lookup ();


    if (init_flg) init_buf(buf1);
    stflg = (oflag & O_TRUNC)?TRUNC:NO_TRUNC;
    if (new)
    {
        put_file( path, bits, buf1 );

        if (!err)
        {
            expecting ( VOID );
            if (stat( path, &st_buf1 ) != 0 )
                severr( "stat before open failed" );
        }

        expecting ( VOID );
        t = time( (ttime *)0) + 2l;

        expecting ( VOID );
        while (time( (ttime *)0) < t);
    }

    expecting ( VOID );
    sts=open( path, oflag, mode );

    if( (err && sts != (-1)) || (!err && sts < 0))
    {
        error("Open returned an unexpected value.\nThe expected value was %d.\nThe actual value was %d.", ((err)?(-1):0), sts);    /*  ERROR */
    }
    else if (err && errno != err)
        error("Open returned an unexpected errno.\nThe expected errno was %s.\n\
The actual errno was %s.", err_lookup (err), err_lookup (errno));

    if (!err && new)
    {
        expecting ( VOID );
        if( stat( path, &st_buf2 ) != 0 )
        {
            severr( "stat of opened file failed" );
        }

        cmp_stat( &st_buf1, &st_buf2, stflg );
        if (!stflg && (bits & 0444))
        {
            get_file( path, buf2 );
            if (cmp_file( buf1, buf2 ) != FILESIZ)
            {
                error("File contents differ.");
            }
        }
    }

    if( sts != (-1) &&  close( sts ) != 0 )
    {
        severr("close failed");
    }

    if ( new && unlink( path ) != 0 )
    {
        severr("unlink failed");
    }
}
