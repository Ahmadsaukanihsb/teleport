local HttpService = game:GetService("HttpService")
local TeleportService = game:GetService("TeleportService")
local Players = game:GetService("Players")
local RunService = game:GetService("RunService")

local player = Players.LocalPlayer
local BACKEND_LATEST_INSTANCE_URL = "https://backend-tau-three-32.vercel.app/api/latestInstance"
local BACKEND_INSTANCE_USERS_URL = "https://backend-tau-three-32.vercel.app/api/instanceUsers"
local TELEPORT_COOLDOWN = 30
local CHAT_DISTANCE_THRESHOLD = 20

local lastTeleportTime = 0
local latestInstanceId = nil

local function getLatestInstance()
    local success, response = pcall(function()
        return HttpService:GetAsync(BACKEND_LATEST_INSTANCE_URL)
    end)
    if success then
        local data = HttpService:JSONDecode(response)
        return data.instanceId
    else
        warn("[AutoJoiner] Failed to get latest instance")
        return nil
    end
end

local function teleportTo(instanceId)
    local now = os.time()
    if instanceId and instanceId ~= game.JobId and now - lastTeleportTime > TELEPORT_COOLDOWN then
        lastTeleportTime = now
        print("[AutoJoiner] Teleporting to instance:", instanceId)
        TeleportService:TeleportToPlaceInstance(game.PlaceId, instanceId, player)
        return true
    end
    return false
end

local function getUsersInInstance(instanceId)
    local url = BACKEND_INSTANCE_USERS_URL .. "?instanceId=" .. instanceId
    local success, response = pcall(function()
        return HttpService:GetAsync(url)
    end)
    if success then
        local data = HttpService:JSONDecode(response)
        return data.users or {}
    else
        warn("[AutoJoiner] Failed to get users in instance")
        return {}
    end
end

local function distanceBetweenPlayers(p1, p2)
    if p1.Character and p2.Character and p1.Character.PrimaryPart and p2.Character.PrimaryPart then
        return (p1.Character.PrimaryPart.Position - p2.Character.PrimaryPart.Position).Magnitude
    end
    return math.huge
end

local function chatIfClose(userAName)
    local userAPlayer = Players:FindFirstChild(userAName)
    if not userAPlayer then
        print("[AutoJoiner] User A not found in server")
        return
    end

    local dist = distanceBetweenPlayers(player, userAPlayer)
    if dist <= CHAT_DISTANCE_THRESHOLD then
        player:Chat("Hello "..userAName.."! We're close, distance: "..math.floor(dist))
    else
        print("[AutoJoiner] Too far to chat:", dist)
    end
end

-- Main loop
while true do
    latestInstanceId = getLatestInstance()

    if latestInstanceId and latestInstanceId ~= game.JobId then
        if teleportTo(latestInstanceId) then
            break -- teleport executed, break loop
        end
    else
        -- Sudah di instance terbaru, cek apakah user A ada
        local users = getUsersInInstance(game.JobId)
        for _, userData in ipairs(users) do
            if userData.username == "UserA" then -- Ganti sesuai username user A
                chatIfClose("UserA")
            end
        end
    end

    task.wait(5)
end
