#!/bin/bash
# Deployment Reliability Validation Script
# This script validates the deployment setup and documentation

set -e

echo "==================================="
echo "Deployment Reliability Validation"
echo "==================================="
echo ""

ERRORS=0
WARNINGS=0

# Color codes
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

print_success() {
    echo -e "${GREEN}✓${NC} $1"
}

print_error() {
    echo -e "${RED}✗${NC} $1"
    ((ERRORS++))
}

print_warning() {
    echo -e "${YELLOW}⚠${NC} $1"
    ((WARNINGS++))
}

# 1. Verify documentation files exist
echo "1. Checking documentation files..."
DOCS=(
    "DEPLOY_STEPS.md"
    "QUICK_DEPLOY_CHECKLIST.md"
    "DEPLOYMENT_SCENARIOS.md"
    "DEPLOYMENT_OVERVIEW.md"
    "DEPLOYMENT_INDEX.md"
    "DEPLOYMENT_SUMMARY.md"
    "README.md"
)

for doc in "${DOCS[@]}"; do
    if [ -f "$doc" ]; then
        print_success "$doc exists"
    else
        print_error "$doc is missing"
    fi
done
echo ""

# 2. Verify GitHub Actions workflow
echo "2. Checking GitHub Actions workflow..."
if [ -f ".github/workflows/build-and-deploy.yml" ]; then
    print_success "Workflow file exists"
    
    # Check for required sections
    if grep -q "build-flutter-apk:" .github/workflows/build-and-deploy.yml; then
        print_success "Build job configured"
    else
        print_error "Build job missing"
    fi
    
    if grep -q "create-release:" .github/workflows/build-and-deploy.yml; then
        print_success "Release job configured"
    else
        print_error "Release job missing"
    fi
    
    if grep -q "permissions:" .github/workflows/build-and-deploy.yml; then
        print_success "Permissions configured"
    else
        print_error "Permissions not configured (security issue)"
    fi
else
    print_error "Workflow file missing"
fi
echo ""

# 3. Verify Flutter project structure
echo "3. Checking Flutter project..."
if [ -f "frontend/flutter project/demo/pubspec.yaml" ]; then
    print_success "pubspec.yaml exists"
    
    # Check version format
    if grep -q '^version: [0-9]' "frontend/flutter project/demo/pubspec.yaml"; then
        VERSION=$(grep '^version:' "frontend/flutter project/demo/pubspec.yaml" | sed 's/version: *//' | cut -d'+' -f1 | tr -d ' ')
        print_success "Version found: $VERSION"
    else
        print_warning "Version format may be invalid"
    fi
else
    print_error "Flutter pubspec.yaml missing"
fi
echo ""

# 4. Verify component READMEs
echo "4. Checking component documentation..."
COMPONENT_DOCS=(
    "backend/google-apps-script/README.md"
    "app/README.md"
    "hardware/README.md"
)

for doc in "${COMPONENT_DOCS[@]}"; do
    if [ -f "$doc" ]; then
        print_success "$doc exists"
    else
        print_warning "$doc missing"
    fi
done
echo ""

# 5. Check for broken internal links
echo "5. Checking internal documentation links..."
LINK_ERRORS=0
for md_file in *.md; do
    if [ -f "$md_file" ]; then
        # Extract local links
        while IFS= read -r link; do
            # Extract path from markdown link
            path=$(echo "$link" | sed -n 's/.*](\.\?\(\/.*\)).*/\1/p' | sed 's/%20/ /g')
            if [ -n "$path" ] && [ "$path" != "$link" ]; then
                # Remove leading ./
                path=${path#./}
                # Remove anchor
                path=${path%%#*}
                if [ ! -f "$path" ] && [ ! -d "$path" ]; then
                    print_warning "Broken link in $md_file: $path"
                    ((LINK_ERRORS++))
                fi
            fi
        done < <(grep -o '\](\.\/[^)]*\|\/[^)]*' "$md_file" 2>/dev/null || true)
    fi
done

if [ $LINK_ERRORS -eq 0 ]; then
    print_success "No broken links found"
fi
echo ""

# 6. Verify directory structure
echo "6. Checking directory structure..."
DIRS=(
    ".github/workflows"
    "backend/google-apps-script"
    "frontend/flutter project/demo"
    "firmware"
    "hardware"
    "app/release"
    "app/debug"
)

for dir in "${DIRS[@]}"; do
    if [ -d "$dir" ]; then
        print_success "$dir exists"
    else
        print_warning "$dir missing"
    fi
done
echo ""

# 7. Check workflow syntax
echo "7. Validating workflow syntax..."
if command -v yamllint &> /dev/null; then
    if yamllint -d '{extends: default, rules: {line-length: disable, trailing-spaces: disable, document-start: disable, truthy: disable}}' .github/workflows/build-and-deploy.yml &> /dev/null; then
        print_success "Workflow YAML syntax valid"
    else
        print_warning "Workflow has linting warnings (non-critical)"
    fi
else
    print_warning "yamllint not installed, skipping syntax check"
fi
echo ""

# 8. Check for security best practices
echo "8. Checking security best practices..."
if grep -q "permissions:" .github/workflows/build-and-deploy.yml; then
    print_success "Workflow has explicit permissions"
else
    print_error "Workflow missing explicit permissions"
fi

if grep -q "secrets.GITHUB_TOKEN" .github/workflows/build-and-deploy.yml; then
    print_success "Using GITHUB_TOKEN correctly"
fi
echo ""

# Summary
echo "==================================="
echo "Validation Summary"
echo "==================================="
if [ $ERRORS -eq 0 ] && [ $WARNINGS -eq 0 ]; then
    echo -e "${GREEN}✓ All checks passed!${NC}"
    echo "Deployment is reliable and ready to use."
    exit 0
elif [ $ERRORS -eq 0 ]; then
    echo -e "${YELLOW}⚠ $WARNINGS warning(s) found${NC}"
    echo "Deployment is functional but has minor issues."
    exit 0
else
    echo -e "${RED}✗ $ERRORS error(s) and $WARNINGS warning(s) found${NC}"
    echo "Please fix errors before deployment."
    exit 1
fi
