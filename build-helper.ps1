# Define directories using Visual Studio macros
$buildDir      = "$(SolutionDir)build"
$targetDir     = "$(TargetDir)"
$targetFile    = "$(TargetFileName)"
$projectDir    = "$(ProjectDir)"

# Ensure the build directory exists
if (-Not (Test-Path $buildDir)) {
    New-Item -ItemType Directory -Path $buildDir | Out-Null
}

# Create the bin subdirectory
$binDir = Join-Path $buildDir "bin"
if (-Not (Test-Path $binDir)) {
    New-Item -ItemType Directory -Path $binDir | Out-Null
}

# Create the res subdirectory
$resDir = Join-Path $buildDir "res"
if (-Not (Test-Path $resDir)) {
    New-Item -ItemType Directory -Path $resDir | Out-Null
}

# Copy the main target file into the bin directory
Copy-Item -Path (Join-Path $targetDir $targetFile) -Destination $binDir -Force

# Copy all .dll files from the target directory into the bin directory
Copy-Item -Path (Join-Path $targetDir "*.dll") -Destination $binDir -Force

# Copy all files from the res directory of the project into the build's res directory
Copy-Item -Path (Join-Path $projectDir "res\*") -Destination $resDir -Force

# Define directories using Visual Studio macros
$buildDir      = "$(SolutionDir)build"
$targetDir     = "$(TargetDir)"
$targetFile    = "$(TargetFileName)"
$projectDir    = "$(ProjectDir)"

# Ensure the build directory exists
if (-Not (Test-Path $buildDir)) {
    New-Item -ItemType Directory -Path $buildDir | Out-Null
}

# Create the bin subdirectory
$binDir = Join-Path $buildDir "bin"
if (-Not (Test-Path $binDir)) {
    New-Item -ItemType Directory -Path $binDir | Out-Null
}

# Create the res subdirectory
$resDir = Join-Path $buildDir "res"
if (-Not (Test-Path $resDir)) {
    New-Item -ItemType Directory -Path $resDir | Out-Null
}

# Copy the main target file into the bin directory
Copy-Item -Path (Join-Path $targetDir $targetFile) -Destination $binDir -Force

# Copy all .dll files from the target directory into the bin directory
Copy-Item -Path (Join-Path $targetDir "*.dll") -Destination $binDir -Force

# Copy all files from the res directory of the project into the build's res directory
Copy-Item -Path (Join-Path $projectDir "res\*") -Destination $resDir -Force