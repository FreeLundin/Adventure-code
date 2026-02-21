<#
create_github_issues.ps1

Create GitHub issues from project task templates using the `gh` CLI.

Prerequisites:
- Install GitHub CLI (`gh`) and authenticate: `gh auth login`.
- Run this script from a shell where `gh` is available.

Usage examples:
  # dry-run (prints commands):
  .\create_github_issues.ps1 -Repo "owner/repo" -DryRun

  # actually create issues:
  .\create_github_issues.ps1 -Repo "owner/repo"

#>

param(
    [Parameter(Mandatory=$true)]
    [string]$Repo,

    [switch]$DryRun
)

$tasks = @(
    @{ Title = 'MM: Export animation manifest for motion-matching'; Labels = 'motion-matching,prototype'; Body = @"
List asset paths, retarget notes, expected feature schema (root_vel, hip_pos, contact_flags), sample CSV produced.

Acceptance: CSV manifest present; per-clip feature exports exist for 10 sample clips.
"@ },

    @{ Title = 'GAS: Implement GA_LightAttack'; Labels = 'gas,ability'; Body = @"
C++/BP ability skeleton, GameplayEffect for cost, tag `ability.light`.

Acceptance: Ability activates, consumes RitualEnergy amount, triggers gameplay cue.
"@ },

    @{ Title = 'GAS: Implement GA_HeavyAttack'; Labels = 'gas,ability'; Body = 'Heavy attack ability skeleton, GameplayEffect cost, and basic combo flow.' },

    @{ Title = 'Animation: Initial retargeting for free packs'; Labels = 'animation,retargeting'; Body = 'Retarget selected free animation packs to project skeleton and document retarget map.' },

    @{ Title = 'Encounters: Implement Grunt enemy AI'; Labels = 'ai,encounters'; Body = 'Simple enemy with patrol, chase, melee attack; used for early combat tuning.' },

    @{ Title = 'Artifact: Create ArtifactData DataAsset template'; Labels = 'data,artifact'; Body = 'DataAsset with fields: Name, Orisha, BaseEffects, SignatureModifier, UpgradeTiers. Include example JSON export.' },

    @{ Title = 'Boss: Prototype multi-phase boss encounter'; Labels = 'boss,encounters'; Body = 'Prototype boss integrating a ritual puzzle phase and combat phases; include a demo map.' }
)

foreach ($t in $tasks) {
    $title = $t.Title
    $body = $t.Body
    $labels = $t.Labels

    $cmd = "gh issue create --repo $Repo --title `"$title`" --body `"$body`" --label `"$labels`""

    if ($DryRun) {
        Write-Output "DRY-RUN: $cmd"
    }
    else {
        Write-Output "Creating issue: $title"
        & gh issue create --repo $Repo --title $title --body $body --label $labels
        if ($LASTEXITCODE -ne 0) {
            Write-Error "Failed to create issue: $title"
            break
        }
    }
}

Write-Output "Done.\nIf you ran with -DryRun, remove that flag to actually create issues."
