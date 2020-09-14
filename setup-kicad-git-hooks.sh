# This script sets up the git triggers to avoid tracking of uneeded file changes as a result of
# kicad. More information can be found here: https://jnavila.github.io/plotkicadsch/ -ECF 2020-09-14

git config --global filter.kicad_project.clean "sed -E 's/^update=.*$/update=Date/'"
git config --global filter.kicad_project.smudge cat

git config --global filter.kicad_sch.clean "sed -E 's/#(PWR|FLG)[0-9]+/#\1?/'"
git config --global filter.kicad_sch.smudge cat
