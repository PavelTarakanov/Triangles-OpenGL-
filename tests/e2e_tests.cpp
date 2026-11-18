#include <gtest/gtest.h>
#include <fstream>
#include <sstream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <sys/wait.h>

namespace {

struct ProcessResult {
    int exit_code;
    std::string output;
};

// Runs the compiled ./out binary with CLI arguments and captures stdout/stderr
ProcessResult run_binary(const std::string& args) {
    const std::string log_file = "e2e_run_output.tmp";
    std::string cmd = "./out " + args + " > " + log_file + " 2>&1";

    int raw_status = std::system(cmd.c_str());
    int exit_code = WIFEXITED(raw_status) ? WEXITSTATUS(raw_status) : -1;

    std::ifstream ifs(log_file);
    std::stringstream buffer;
    buffer << ifs.rdbuf();
    ifs.close();
    std::remove(log_file.c_str());

    return {exit_code, buffer.str()};
}

// RAII helper to create and automatically clean up temporary scene files
class TempSceneFile {
private:
    std::string path_;

public:
    TempSceneFile(const std::string& path, const std::string& content) : path_(path) {
        std::ofstream ofs(path_);
        ofs << content;
    }

    ~TempSceneFile() {
        std::remove(path_.c_str());
    }

    const std::string& path() const { return path_; }
};

} // namespace

TEST(E2EBlackBoxTest, NonCoplanarTrianglesIntersection) {
    // Assign: First triangle lies in XY plane; second crosses it vertically along (0.5, 0, 0) -> (0.5, 1, 0)
    TempSceneFile scene(
        "e2e_non_coplanar.txt",
        "2\n"
        "0.0 0.0 0.0  2.0 0.0 0.0  0.0 2.0 0.0\n"
        "0.5 -0.5 1.0  0.5 1.5 1.0  0.5 0.5 -1.0\n"
    );

    // Act
    ProcessResult res = run_binary("--headless " + scene.path());

    // Assert
    EXPECT_EQ(res.exit_code, 0);
    EXPECT_NE(res.output.find("TRIANGLES: 2"), std::string::npos);
    EXPECT_NE(res.output.find("COPLANAR_POLYGONS: 0"), std::string::npos);
    EXPECT_NE(res.output.find("NON_COPLANAR_SEGMENTS: 1"), std::string::npos);
    EXPECT_NE(res.output.find("SEGMENT: (0.5, 0, 0) -> (0.5, 1, 0)"), std::string::npos);
}

TEST(E2EBlackBoxTest, CoplanarOverlappingTriangles) {
    // Assign: Two triangles lie in the same Z = 0 plane and overlap
    TempSceneFile scene(
        "e2e_coplanar.txt",
        "2\n"
        "0.0 0.0 0.0  4.0 0.0 0.0  0.0 4.0 0.0\n"
        "1.0 1.0 0.0  5.0 1.0 0.0  1.0 5.0 0.0\n"
    );

    // Act
    ProcessResult res = run_binary("--headless " + scene.path());

    // Assert
    EXPECT_EQ(res.exit_code, 0);
    EXPECT_NE(res.output.find("TRIANGLES: 2"), std::string::npos);
    EXPECT_NE(res.output.find("COPLANAR_POLYGONS: 1"), std::string::npos);
    EXPECT_NE(res.output.find("NON_COPLANAR_SEGMENTS: 0"), std::string::npos);
}

TEST(E2EBlackBoxTest, DisjointTrianglesCulledByAABB) {
    // Assign: Triangles are far apart and culled during AABB broad-phase check
    TempSceneFile scene(
        "e2e_disjoint.txt",
        "2\n"
        "0.0 0.0 0.0  1.0 0.0 0.0  0.0 1.0 0.0\n"
        "10.0 10.0 10.0  12.0 10.0 10.0  10.0 12.0 10.0\n"
    );

    // Act
    ProcessResult res = run_binary("--headless " + scene.path());

    // Assert
    EXPECT_EQ(res.exit_code, 0);
    EXPECT_NE(res.output.find("COPLANAR_POLYGONS: 0"), std::string::npos);
    EXPECT_NE(res.output.find("NON_COPLANAR_SEGMENTS: 0"), std::string::npos);
}

TEST(E2EBlackBoxTest, StdinInputPipeline) {
    // Assign: Feed scene data via standard input (stdin) without passing a file path
    const std::string log_file = "e2e_stdin_output.tmp";
    std::string cmd = "printf \"2\\n0 0 0 2 0 0 0 2 0\\n0 0 0 2 0 0 0 2 0\\n\" | ./out --headless > " + log_file + " 2>&1";

    // Act
    int raw_status = std::system(cmd.c_str());
    int exit_code = WIFEXITED(raw_status) ? WEXITSTATUS(raw_status) : -1;

    std::ifstream ifs(log_file);
    std::stringstream buffer;
    buffer << ifs.rdbuf();
    std::string output = buffer.str();
    ifs.close();
    std::remove(log_file.c_str());

    // Assert
    EXPECT_EQ(exit_code, 0);
    EXPECT_NE(output.find("COPLANAR_POLYGONS: 1"), std::string::npos);
}

TEST(E2EBlackBoxTest, MissingInputFileReturnsError) {
    // Act
    ProcessResult res = run_binary("--headless definitely_non_existent_scene.txt");

    // Assert
    EXPECT_EQ(res.exit_code, 1);
    EXPECT_NE(res.output.find("Error: Could not open file"), std::string::npos);
}

TEST(E2EBlackBoxTest, CorruptedVertexDataReturnsError) {
    // Assign: Header specifies 2 triangles, but vertex data is provided for only 1
    TempSceneFile scene(
        "e2e_corrupted.txt",
        "2\n"
        "0.0 0.0 0.0  1.0 0.0 0.0  0.0 1.0 0.0\n"
    );

    // Act
    ProcessResult res = run_binary("--headless " + scene.path());

    // Assert
    EXPECT_EQ(res.exit_code, 1);
    EXPECT_NE(res.output.find("Error: Incomplete vertex data"), std::string::npos);
}
