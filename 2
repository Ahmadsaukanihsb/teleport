local HttpService = game:GetService("HttpService")
local Players = game:GetService("Players")

local player = Players.LocalPlayer
local BACKEND_URL = "https://backend-vercel-ashy.vercel.app/api/register.js"
local WEBHOOK_URL = "https://discord.com/api/webhooks/1378086156624990361/8qHKxSBQ8IprT1qFn1KkHDWsyRfKXPJkS_4OYzMkBC-PIhGClm0v36uIgzrVwtU1zXh6"  -- Ganti dengan webhook Anda
local RETRY_LIMIT = 3
local RETRY_DELAY = 2
local DEBOUNCE_TIME = 30

local lastSendTime = 0

-- Fungsi untuk mengirim ke backend Vercel
local function sendToBackend(data)
    local success, response = pcall(function()
        return HttpService:RequestAsync({
            Url = BACKEND_URL,
            Method = "POST",
            Headers = {
                ["Content-Type"] = "application/json",
                ["X-Game-ID"] = game.GameId
            },
            Body = HttpService:JSONEncode(data)
        })
    end)
    
    if not success then
        warn("[Backend] Request failed:", response)
        return false
    end
    
    return response.StatusCode == 200
end

-- Fungsi untuk mengirim ke webhook (Discord)
local function sendToWebhook(data)
    local embed = {
        {
            title = "Player Joined",
            description = string.format(
                "**%s** joined instance\n**UserID**: %d\n**Game**: [%d](https://www.roblox.com/games/%d)",
                data.username, data.userId, data.placeId, data.placeId
            ),
            color = 65280,  -- Warna hijau
            fields = {
                {
                    name = "Instance ID",
                    value = data.instanceId,
                    inline = true
                },
                {
                    name = "Timestamp",
                    value = os.date("%Y-%m-%d %H:%M:%S", data.timestamp),
                    inline = true
                }
            },
            footer = {
                text = "Tracker System"
            }
        }
    }
    
    local success, response = pcall(function()
        return HttpService:RequestAsync({
            Url = WEBHOOK_URL,
            Method = "POST",
            Headers = {["Content-Type"] = "application/json"},
            Body = HttpService:JSONEncode({
                content = nil,
                embeds = embed
            })
        })
    end)
    
    if not success then
        warn("[Webhook] Request failed:", response)
        return false
    end
    
    return response.StatusCode == 204
end

-- Fungsi utama dengan retry mechanism
local function sendData()
    if os.time() - lastSendTime < DEBOUNCE_TIME then
        return false
    end
    
    local data = {
        username = player.Name,
        userId = player.UserId,
        instanceId = game.JobId,
        timestamp = os.time(),
        placeId = game.PlaceId,
        gameId = game.GameId
    }
    
    lastSendTime = os.time()
    
    -- Kirim ke kedua tujuan secara parallel
    task.spawn(function()
        for i = 1, RETRY_LIMIT do
            if sendToBackend(data) then break end
            if i < RETRY_LIMIT then task.wait(RETRY_DELAY) end
        end
    end)
    
    task.spawn(function()
        for i = 1, RETRY_LIMIT do
            if sendToWebhook(data) then break end
            if i < RETRY_LIMIT then task.wait(RETRY_DELAY) end
        end
    end)
    
    return true
end

-- Sistem event handlers
local function init()
    if player.Character then
        sendData()
    else
        player.CharacterAdded:Connect(sendData)
    end
    
    -- Periodic update (opsional)
    while true do
        task.wait(300)  -- Kirim ulang setiap 5 menit
        sendData()
    end
end

-- Inisialisasi
task.spawn(init)

-- Handle disconnect
game:GetService("NetworkClient").Disconnection:Connect(function()
    sendData()
    print("[Logger] Final data sent before disconnect")
end)
