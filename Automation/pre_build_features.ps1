# Pre-build step: must succeed before build proceeds
Write-Host "[SVGLND] Running pre-build motion feature extraction"

Engine/Binaries/Win64/UnrealEditor.exe SVGLND.uproject `
  -ExecCmds="Automation RunTests SVGLND.MotionFeatures.PerFrame; Quit" `
  -unattended -nop4 -nosplash -NoSound

if ($LASTEXITCODE -ne 0) {
  Write-Error "[SVGLND] Motion feature extraction failed"
  exit 1
}

python Automation/features_manifest_updater.py
