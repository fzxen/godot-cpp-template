#!/bin/bash

GODOT_APP="${GODOT_PATH}"

echo "Resigning $GODOT_APP"

if [ -z "$GODOT_APP" ] || [ ! -d "$GODOT_APP" ]; then
    echo "Error: No Godot path provided or path does not exist"
    exit 1
fi

ENTITLEMENTS_FILE="/tmp/godot_debug.entitlements"

# 判断是否已经重新签名过，若已签名则直接退出
if codesign -d --entitlements - "$GODOT_APP" 2>&1 | grep -q "com.apple.security.cs.debugger"; then
    echo "Warning: $GODOT_APP is already signed with debug entitlements"
    exit 0
fi

echo "=== Step 1: Remove existing signature ==="
codesign --remove-signature "$GODOT_APP"
if [ $? -ne 0 ]; then
    echo "Warning: Failed to remove signature (may not have one, continuing)"
fi

echo "=== Step 2: Create entitlements file ==="
cat > "$ENTITLEMENTS_FILE" << 'EOF'
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>com.apple.security.get-task-allow</key>
    <true/>
    <key>com.apple.security.cs.disable-library-validation</key>
    <true/>
    <key>com.apple.security.cs.debugger</key>
    <true/>
</dict>
</plist>
EOF

echo "=== Step 3: Re-sign ==="
codesign --force --deep --sign - --entitlements "$ENTITLEMENTS_FILE" "$GODOT_APP"

if [ $? -eq 0 ]; then
    echo "=== Signing successful ==="
    echo "Verifying signature:"
    codesign -dv "$GODOT_APP" 2>&1 | head -5
    echo ""
    echo "Verifying entitlements:"
    codesign -d --entitlements - "$GODOT_APP" 2>&1 | grep -E "(get-task-allow|disable-library-validation)" || echo "Entitlements not found"
else
    echo "=== Signing failed ==="
    exit 1
fi

# Clean up temporary files
rm -f "$ENTITLEMENTS_FILE"