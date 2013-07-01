#!/bin/bash

exec > >(tee -a "data.log")

until ./zenith; do
    echo "Zenith crashed with exit code $?.  Respawning.."
    sleep 1
done