#!/usr/bin/perl

#-----------------------------------------------------------------------------
# Copyright (c) 2001-2004 Swedish Institute of Computer Science.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
# 3. The name of the author may not be used to endorse or promote products
#    derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO
# EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
# OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
# WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
# OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
# ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
#
# This file is part of the lwIP TCP/IP stack.
#
# Author: Adam Dunkels <adam@sics.se>
#
#-----------------------------------------------------------------------------
#-----------------------------------------------------------------------------
# Copyright (c) 2009 Quantum Leaps, LLC, www.state-machine.com
#
# Cleaned the script, made it Windows-compatible, and improved the formatting
# of the generated C code.
#-----------------------------------------------------------------------------
print("Usage: perl makefsdata.pl [root dir [output file]] [-h]\n");

my $rootdir = $ARGV[0];
my $outfile = $ARGV[1];
my $nohtml  = $ARGV[2];

if ($rootdir eq '-h') {
    $nohtml  = '-h';
    $rootdir = 'fs';
}
elsif ($rootdir eq '') {
    $rootdir = 'fs';
}

if ($outfile eq '-h') {
    $nohtml  = '-h';
    $outfile = 'fsdata.h';
}
elsif ($outfile eq '') {
    $outfile = 'fsdata.h';
}

open(OUTPUT, "> " . $outfile);
chdir($rootdir);
open(FILES, "find . -type f |");

print("Generating: $outfile...\n");

print(OUTPUT "/* This file has been generated");
print(OUTPUT " with the makefsdata.pl perl script. */\n\n");

while ($file = <FILES>) {

    # Do not include files in CVS directories nor backup files
    if ($file =~ /(CVS|~)/) {
        next;
    }

    chop($file);
    open(FILE, $file);

    $file =~ s/\.//;
    $fvar = $file;
    $fvar =~ s-/-_-g;
    $fvar =~ s-\\-_-g;
    $fvar =~ s-\.-_-g;

    # encode the file name
    print("    Adding: $file...\n");
    print(OUTPUT "/* $file */\n");
    print(OUTPUT "static unsigned char const data" . $fvar . "[] = {\n");
    print(OUTPUT "    /* name: */\n");
    $i = 0;
    for ($j = 0; $j < length($file); $j++) {
        if ($i == 0) {
            print(OUTPUT "    ");
        }
        printf(OUTPUT "0x%02X, ", unpack("C", substr($file, $j, 1)));
        $i++;
        if ($i == 10) {
            print(OUTPUT "\n");
            $i = 0;
        }
    }
    if ($i == 0) {
        print(OUTPUT "    ");
    }
    print(OUTPUT "0x00,\n");

    # encode the HTML header, if command-line option -h NOT provided
    if (!($nohtml eq '-h')) {
		if ($file =~ /404/) {
		    $header = "HTTP/1.0 404 File not found\r\n";
		}
		else {
			$header = "HTTP/1.0 200 OK\r\n";
		}
		$header = $header .	"Server: QP-lwIP/1.3.0";
		$header = $header .	" (http://www.state-machine.com)\r\n";
		if (($file =~ /\.html$/) || ($file =~ /\.htm$/)) {
			$header = $header .	"Content-type: text/html\r\n";
		}
		elsif ($file =~ /\.gif$/) {
			$header = $header .	"Content-type: image/gif\r\n";
		}
		elsif ($file =~ /\.png$/) {
			$header = $header .	"Content-type: image/png\r\n";
		}
		elsif ($file =~ /\.jpg$/) {
			$header = $header .	"Content-type: image/jpeg\r\n";
		}
		elsif ($file =~ /\.class$/) {
			$header = $header .	"Content-type: application/octet-stream\r\n";
		}
		elsif ($file =~ /\.ram$/) {
			$header = $header .	"Content-type: audio/x-pn-realaudio\r\n";
		}
		elsif ($file =~ /\.bmp$/) {
			$header = $header .	"Content-type: image/bmp\r\n";
		}
		elsif ($file =~ /\.css$/) {
			$header = $header .	"Content-type: text/css\r\n";
		}
		else {
			$header = $header .	"Content-type: text/plain\r\n";
		}
		$header = $header .	"\r\n";

	    print(OUTPUT "    /* HTML header: */\n");
	    $i = 0;
	    for ($j = 0; $j < length($header); $j++) {
	        if ($i == 0) {
	            print(OUTPUT "    ");
	        }
	        printf(OUTPUT "0x%02X, ", unpack("C", substr($header, $j, 1)));
	        $i++;
	        if ($i == 10) {
	            print(OUTPUT "\n");
	            $i = 0;
	        }
	    }
		if ($i != 0) {
	        print(OUTPUT "\n");
		}
	}

    # encode the file data
    print(OUTPUT "    /* data: */\n");
    $i = 0;
    while (read(FILE, $data, 1)) {
        if ($i == 0) {
            print(OUTPUT "    ");
        }
        printf(OUTPUT "0x%02X, ", unpack("C", $data));
        $i++;
        if ($i == 10) {
            print(OUTPUT "\n");
            $i = 0;
        }
    }
    print(OUTPUT "\n};\n\n");
    close(FILE);
    push(@fvars, $fvar);
    push(@files, $file);
}

# encode the file-system declarations
print("    Adding: declarations...\n");
for ($i = 0; $i < @fvars; $i++) {
    $file = $files[$i];
    $fvar = $fvars[$i];

    if ($i == 0) {
        $prevfile = "(struct fsdata_file *)0";
    }
    else {
        $prevfile = "file" . $fvars[$i - 1];
    }
    print(OUTPUT "struct fsdata_file const file".$fvar."[] = {\n    {\n");
    print(OUTPUT "        $prevfile,\n");
    print(OUTPUT "        data$fvar,\n");
    print(OUTPUT "        data$fvar + ". (length($file) + 1) .",\n");
    print(OUTPUT "        sizeof(data$fvar) - ". (length($file) + 1) ."\n");
    print(OUTPUT "    }\n};\n\n");
}

print(OUTPUT "#define FS_ROOT file$fvars[$i - 1]\n\n");
print(OUTPUT "#define FS_NUMFILES $i\n");

print("Done.");
