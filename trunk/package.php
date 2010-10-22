<?php

/** Recursive mkdir(). */
function mkdirR( $path, $sep='/' )
{
	$p = '';
	foreach( explode($sep,$path) as $c )
	{
		$c = trim($c);
		if ( $c != '' )
		{
			$newdir = $p.$c;
			//echo "Making dir $newdir\n";
			if ( !is_dir($newdir) && !mkdir($newdir) )
				die( "Failed to create dir $newdir\n" );
			$p .= $c.'/';
		}
	}
}

/** Copies source file(s) to destination directory. Supports * as wildcard. */
function copyFiles( $src, $dest, $sep='/' )
{
	if ( substr($dest,-1) != $sep )
		$dest .= $sep;
		
	$srcdir = dirname($src);
	$files = array( basename($src) );
	//echo "Copying $files from $srcdir to $dest\n";
	if ( $files[0] == '*' )
		$files = scandir( $srcdir );

	foreach ($files as $file)
	{
		$srcfile = $srcdir.$sep.$file;
		if ( $file != '.' && $file != '..' && is_file($srcfile) )
		{
			$destfile = $dest.$file;
			if ( !copy( $srcfile, $destfile ) )
				die( "Failed to copy $srcfile to $destfile\n" );
		}
	}
}

// first line: slmath ver. VERSION
$readme = file('readme.txt');
if ( 1 != preg_match( '/slmath ver\. (.+)\s/', $readme[0], $m ) )
	die( "Version grab from readme.txt failed" );
$ver = trim($m[1]);

// target dir for package
$basedir = "slmath-$ver/";
mkdirR( $basedir );

// package files listed in package.lst
$files = file( 'package.lst' );
foreach ($files as $file)
{
	$file = trim($file);
	
	$dst = $basedir;
	$dirname = dirname( $file );
	if ( $dirname != '.' )
	{
		$dst .= $dirname;
		mkdirR( $dst );
	}

	//echo $dst."\n";
	copyFiles( $file, $dst );
}
chdir( $basedir );
system( "pkzip -add -dir slmath-$ver.zip" );

?>