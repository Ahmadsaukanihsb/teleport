repeat task.wait() until game:IsLoaded()

local HttpService = game:GetService("HttpService")
local TeleportService = game:GetService("TeleportService")
local Players = game:GetService("Players")

local player = Players.LocalPlayer
local PLACE_ID = game.PlaceId

-- Ganti URL ini dengan endpoint backend kamu
local API_URL = "http://backend-vercel-ashy.vercel.app/api/latest-user.js"

-- Fungsi ambil data dari backend
local function getLatestUserAndInstance()
    local success, response = pcall(function()
        return game:HttpGet(API_URL)
    end)

    if not success then
        warn("[AutoJoiner] Gagal ambil data dari API:", response)
        return nil
    end

    local ok, data = pcall(function()
        return HttpService:JSONDecode(response)
    end)

    if ok and data and data.instanceId then
        return data.username, data.instanceId
    else
        warn("[AutoJoiner] Gagal decode JSON atau tidak ada instanceId")
        return nil
    end
end

-- Fungsi teleport ke instance
local function teleportToInstance(instanceId)
    if not instanceId or instanceId == "" then
        warn("[AutoJoiner] instanceId kosong, tidak bisa teleport")
        return
    end

    warn("[AutoJoiner] Teleporting ke instance:", instanceId)
    TeleportService:TeleportToPlaceInstance(PLACE_ID, instanceId, player)
end

-- Eksekusi utama
local username, instanceId = getLatestUserAndInstance()
if instanceId then
    teleportToInstance(instanceId)
else
    warn("[AutoJoiner] Tidak ada instanceId yang valid, teleport dibatalkan")
end
