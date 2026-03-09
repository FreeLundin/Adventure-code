#!/usr/bin/env python
"""Very simple training pipeline stub.
Reads the manifest CSV produced by the motion_matching_pipeline script and
prints basic statistics that a real training system would consume.
"""
import csv
import os

proj = os.path.abspath(os.path.join(os.path.dirname(__file__), '..'))
manifest = os.path.join(proj, 'Saved', 'AnimManifest.csv')

if not os.path.exists(manifest):
    print('Manifest not found, run motion_matching_pipeline first.')
    exit(1)

clips = []
with open(manifest, newline='') as f:
    reader = csv.DictReader(f)
    for row in reader:
        clips.append(row)

print(f'Loaded {len(clips)} clips from manifest.')

# placeholder: compute simple search kernel (e.g. average length)
avg_len = sum(float(c['Length']) for c in clips) / len(clips) if clips else 0
print('Average clip length:', avg_len)

# write out a "kernel" file
kernel_path = os.path.join(proj, 'Saved', 'MM_Kernel.txt')
with open(kernel_path, 'w') as kf:
    kf.write(f'AverageLength,{avg_len}\n')

print('Training kernel written to', kernel_path)
