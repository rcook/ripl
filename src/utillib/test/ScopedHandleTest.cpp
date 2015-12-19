#include "catch.hpp"
#include "utillib/ScopedHandle.h"
#include <cassert>
#include <unordered_map>

using namespace std;

enum class HandleState
{
    Created,
    Opened,
    Closed
};

using RawHandle = void*;

class HandleInfo
{
public:
    HandleInfo() = delete;
    HandleInfo(const HandleInfo&) = delete;
    HandleInfo& operator=(const HandleInfo&) = delete;

public:
    ~HandleInfo() = default;

    HandleInfo(HandleInfo&& other)
        : m_rawHandle(move(other.m_rawHandle))
        , m_name(move(other.m_name))
        , m_state(move(other.m_state))
    {
    }

    HandleInfo(RawHandle rawHandle, const string& name)
        : m_rawHandle(rawHandle)
        , m_name(name)
        , m_state(HandleState::Created)
    {
    }

    RawHandle rawHandle() const { return m_rawHandle; }
    const string& name() const { return m_name; }
    HandleState state() const { return m_state; }
    void setState(HandleState state) { m_state = state; }

private:
    const RawHandle m_rawHandle;
    const string m_name;
    HandleState m_state;
};

class HandleUniverse
{
public:
    HandleUniverse(const HandleUniverse&) = delete;
    HandleUniverse& operator=(const HandleUniverse&) = delete;

public:
    HandleUniverse() = default;

    ~HandleUniverse()
    {
        for (const auto& pair : m_handleInfos)
        {
            REQUIRE(pair.second.state() == HandleState::Closed);
        }
    }

    RawHandle openRawHandle(const string& name)
    {
        auto rawHandle = reinterpret_cast<void*>(m_handleInfos.size() + 100);
        m_handleInfos.emplace(rawHandle, HandleInfo(rawHandle, name));

        auto& handleInfo = m_handleInfos.at(rawHandle);
        handleInfo.setState(HandleState::Opened);
        return rawHandle;
    }

    const string& getRawHandleName(RawHandle rawHandle) const
    {
        auto& handleInfo = m_handleInfos.at(rawHandle);
        assert(handleInfo.state() == HandleState::Opened);
        return handleInfo.name();
    }

    HandleState getRawHandleState(RawHandle rawHandle) const
    {
        auto& handleInfo = m_handleInfos.at(rawHandle);
        return handleInfo.state();
    }

    void closeRawHandle(RawHandle rawHandle)
    {
        auto& handleInfo = m_handleInfos.at(rawHandle);
        assert(handleInfo.state() == HandleState::Opened);
        handleInfo.setState(HandleState::Closed);
    }

private:
    unordered_map<RawHandle, HandleInfo> m_handleInfos;
};

using CloseFunc = function<void(RawHandle)>;
using TestHandle = ScopedHandle<RawHandle, nullptr, CloseFunc>;
using TestUniquePtr = unique_ptr<remove_pointer<RawHandle>::type, CloseFunc>;

static TestHandle openTestHandle(HandleUniverse& universe, const string& name)
{
    // Uses move constructor
    TestHandle handle(
        universe.openRawHandle(name),
        [&universe](RawHandle rawHandle) { universe.closeRawHandle(rawHandle); });
    return handle;
}

TEST_CASE("ScopedHandle", "ScopedHandle")
{
    HandleUniverse universe;
    CloseFunc closeFunc = [&universe](RawHandle rawHandle)
    {
        universe.closeRawHandle(rawHandle);
    };

    SECTION("invalid")
    {
        TestHandle handle(closeFunc);

        REQUIRE(!handle);

        bool handleAsBool = handle;
        REQUIRE(!handleAsBool);

        REQUIRE_THROWS_AS(handle.get(), runtime_error);
    }

    SECTION("valid")
    {
        auto handle = openTestHandle(universe, "valid");

        REQUIRE(handle);

        bool handleAsBool = handle;
        REQUIRE(handleAsBool);

        REQUIRE_NOTHROW(handle.get());

        REQUIRE(handle.get());

        REQUIRE(universe.getRawHandleState(handle.get()) == HandleState::Opened);
    }

    SECTION("move")
    {
        vector<RawHandle> rawHandles;

        // Check that handle is moved between ScopeHandle instances correctly
        {
            auto handle = openTestHandle(universe, "move");
            rawHandles.push_back(handle.get());
            REQUIRE(universe.getRawHandleName(handle.get()).compare("move") == 0);
            REQUIRE(universe.getRawHandleState(handle.get()) == HandleState::Opened);
        }

        for (auto rawHandle : rawHandles)
        {
            REQUIRE(universe.getRawHandleState(rawHandle) == HandleState::Closed);
        }
    }

    SECTION("swap")
    {
        auto handle0 = openTestHandle(universe, "swap0");
        auto rawHandle0 = handle0.get();

        auto handle1 = openTestHandle(universe, "swap1");
        auto rawHandle1 = handle1.get();

        REQUIRE(rawHandle0 != rawHandle1);

        handle0.swap(handle1);

        REQUIRE(handle0.get() == rawHandle1);
        REQUIRE(universe.getRawHandleState(handle0.get()) == HandleState::Opened);

        REQUIRE(handle1.get() == rawHandle0);
        REQUIRE(universe.getRawHandleState(handle1.get()) == HandleState::Opened);
    }

    SECTION("release")
    {
        auto handle = openTestHandle(universe, "release");
        auto rawHandle = handle.get();
        REQUIRE(universe.getRawHandleState(handle.get()) == HandleState::Opened);

        TestUniquePtr ptr(handle.release(), closeFunc);

        auto releasedRawHandle = ptr.get();
        REQUIRE(releasedRawHandle == rawHandle);
        REQUIRE_THROWS_AS(handle.get(), runtime_error);

        REQUIRE(universe.getRawHandleState(rawHandle) == HandleState::Opened);
    }

    SECTION("reset")
    {
        SECTION("invalid")
        {
            auto handle = openTestHandle(universe, "reset-invalid");
            auto rawHandle = handle.get();

            REQUIRE(universe.getRawHandleState(rawHandle) == HandleState::Opened);

            handle.reset();

            REQUIRE(universe.getRawHandleState(rawHandle) == HandleState::Closed);
        }

        SECTION("other")
        {
            TestUniquePtr ptr0(universe.openRawHandle("reset-other0"), closeFunc);
            auto rawHandle0 = ptr0.get();

            auto handle1 = openTestHandle(universe, "reset-other1");
            auto rawHandle1 = handle1.get();

            REQUIRE(universe.getRawHandleState(rawHandle0) == HandleState::Opened);
            REQUIRE(universe.getRawHandleState(rawHandle1) == HandleState::Opened);

            handle1.reset(ptr0.release());

            REQUIRE(universe.getRawHandleState(rawHandle0) == HandleState::Opened);
            REQUIRE(universe.getRawHandleState(rawHandle1) == HandleState::Closed);
        }
    }
}
