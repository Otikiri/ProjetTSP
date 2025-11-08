#!/bin/bash
set -e

# Ensure we are in the directory of the script
cd "$(dirname "$0")"

# Create virtual environment if it doesn't exist
if [ ! -d "venv" ]; then
    echo "Creating clean virtual environment..."
    python3 -m venv --without-pip venv
    source venv/bin/activate
    curl -sS https://bootstrap.pypa.io/get-pip.py | python
fi

# Activate the virtual environment
source venv/bin/activate

# Upgrade pip
pip install --upgrade pip

# Install packages if requirements.txt exists
if [ -f "requirements.txt" ]; then
    # Remove any Windows ADS entries like getopt or :Zone.Identifier
    grep -vE "getopt|:Zone.Identifier" requirements.txt > temp_requirements.txt
    pip install -r temp_requirements.txt
    rm temp_requirements.txt
else
    echo "Warning: requirements.txt not found, skipping dependencies installation."
fi

echo "Environment setup is complete. Use 'source venv/bin/activate' to activate the virtual environment."
