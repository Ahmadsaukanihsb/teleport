
-- Chat bermasalah


repeat task.wait() until game:IsLoaded()

local HttpService = game:GetService("HttpService")
local Players = game:GetService("Players")
local TeleportService = game:GetService("TeleportService")
local RunService = game:GetService("RunService")
local LocalPlayer = Players.LocalPlayer

local PLACE_ID = game.PlaceId
local BACKEND_URL = "http://backend-vercel-ashy.vercel.app/api/latest-user.js"

local function fetchLatestUserData()
    local success, response = pcall(function()
        return game:HttpGet(BACKEND_URL)
    end)

    if not success then
        warn("[AutoJoin] Gagal fetch data dari backend:", response)
        return nil
    end

    local ok, data = pcall(function()
        return HttpService:JSONDecode(response)
    end)

    if ok and data and data.username and data.instanceId then
        return data.username, data.instanceId
    else
        warn("[AutoJoin] Data tidak valid dari backend")
        return nil
    end
end

local function teleportToInstance(instanceId)
    warn("[AutoJoin] Teleporting ke instance:", instanceId)
    TeleportService:TeleportToPlaceInstance(PLACE_ID, instanceId, LocalPlayer)
end

-- Cek apakah kita sudah di instance yang dituju
local function isInTargetInstance(targetInstanceId)
    return game.JobId == targetInstanceId
end

-- Cek apakah target player berada di server
local function getTargetPlayer(username)
    return Players:FindFirstChild(username)
end

-- Hitung jarak antara 2 player
local function getDistanceFromPlayer(target)
    local myChar = LocalPlayer.Character
    local targetChar = target and target.Character
    if myChar and targetChar and myChar:FindFirstChild("HumanoidRootPart") and targetChar:FindFirstChild("HumanoidRootPart") then
        local pos1 = myChar.HumanoidRootPart.Position
        local pos2 = targetChar.HumanoidRootPart.Position
        return (pos1 - pos2).Magnitude
    end
    return math.huge
end

-- Chat loop
local function startChatLoop(targetUsername)
    local stop = false
    local lastMessageTime = 0

    task.spawn(function()
        while not stop do
            local targetPlayer = getTargetPlayer(targetUsername)

            if not targetPlayer then
                warn("[AutoChat] Target player tidak ditemukan di server.")
                stop = true
                break
            end

            local distance = getDistanceFromPlayer(targetPlayer)
            if distance <= 20 then
                warn("[AutoChat] Jarak terlalu dekat. Menghentikan chat.")
                stop = true
                break
            end

            if tick() - lastMessageTime >= 10 then
                local msg = string.format("Hai %s 👋", targetUsername)
                game.ReplicatedStorage.DefaultChatSystemChatEvents.SayMessageRequest:FireServer(msg, "All")
                lastMessageTime = tick()
                warn("[AutoChat] Mengirim pesan ke:", targetUsername)
            end

            task.wait(5)
        end
    end)
end

-- Eksekusi utama
local targetUsername, targetInstanceId = fetchLatestUserData()

if not targetUsername or not targetInstanceId then
    warn("[AutoJoin] Data tidak valid, proses dibatalkan")
    return
end

if not isInTargetInstance(targetInstanceId) then
    teleportToInstance(targetInstanceId)
else
    warn("[AutoJoin] Sudah berada di instance target.")
    startChatLoop(targetUsername)
end
