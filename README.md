#include "core/PackageVersion.h"

#include <cassert>

int main() {
    using ultrapkg::PackageVersion;

    assert(PackageVersion("1.0.0") == PackageVersion("1.0.0"));
    assert(PackageVersion("1.0.0") < PackageVersion("1.0.1"));
    assert(PackageVersion("2.0") > PackageVersion("1.9.9"));
    assert(PackageVersion("v0.1.0") == PackageVersion("0.1.0"));
    assert(PackageVersion("1.0~rc1") < PackageVersion("1.0"));
    assert(PackageVersion("1.0-1") > PackageVersion("1.0"));

    return 0;
}
