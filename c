local Players = game:GetService("Players")
local ReplicatedStorage = game:GetService("ReplicatedStorage")
local TextChatService = game:GetService("TextChatService")

local localPlayer = Players.LocalPlayer
local GIFT_COOLDOWN = 3
local lastGiftCheck = 0

-- Enhanced function to find gift-related remotes
local function findGiftRemote(names)
    for _, name in ipairs(names) do
        local remote = ReplicatedStorage:FindFirstChild(name)
        if remote then return remote end
    end
    return nil
end

-- Improved function to accept all types of gifts
local function acceptAllGifts()
    -- Try common gift remote names
    local giftRemotes = {
        "GiftSystemRemote",
        "GiftEvent",
        "ClaimGifts",
        "GiftHandler",
        "GiftManager",
        "ReceiveGifts",
        "GiftCollector"
    }
    
    local remote = findGiftRemote(giftRemotes)
    if not remote then return false end

    -- Try different parameter formats
    local formats = {
        "ClaimAll",
        "AcceptAllGifts",
        "CollectAll",
        {Action = "ClaimAllGifts"},
        {Action = "CollectAll", UserId = localPlayer.UserId},
        {Cmd = "ClaimAll", Player = localPlayer}
    }

    for _, format in ipairs(formats) do
        local success = pcall(function()
            if remote:IsA("RemoteEvent") then
                remote:FireServer(format)
            elseif remote:IsA("RemoteFunction") then
                remote:InvokeServer(format)
            end
            return true
        end)
        if success then return true end
    end
    
    return false
end

-- Function to check for visible gift notifications
local function checkForGiftNotifications()
    -- Check if there are any gift notifications in the UI
    local gui = localPlayer:FindFirstChildOfClass("PlayerGui")
    if not gui then return false end
    
    -- Common notification names
    local notificationNames = {
        "GiftNotification",
        "NewGiftAlert",
        "GiftPopup",
        "RewardNotification",
        "GiftIcon"
    }
    
    for _, name in ipairs(notificationNames) do
        local notification = gui:FindFirstChild(name, true)
        if notification and notification.Visible then
            return true
        end
    end
    
    return false
end

-- Main collection function
local function collectAllGifts()
    if os.time() - lastGiftCheck < GIFT_COOLDOWN then return end
    lastGiftCheck = os.time()

    print("\n[System] Scanning for all gift types...")
    
    -- First check if there are visible notifications
    local hasNotifications = checkForGiftNotifications()
    
    -- Try to accept all gifts
    local success = acceptAllGifts()
    
    if success then
        print("[Success] All available gifts claimed")
    elseif hasNotifications then
        print("[Warning] Gifts detected but couldn't claim automatically - may require manual collection")
    else
        print("[Info] No gifts available for collection")
    end

-- Auto-execute with better timing
local function main()
    print("Auto-gift collector initialized")
    while task.wait(GIFT_COOLDOWN) do
        local success, err = pcall(collectAllGifts)
        if not success then
            warn("[Error] During gift collection:", err)
            -- Increase cooldown if errors occur frequently
            task.wait(GIFT_COOLDOWN * 2)
        end
    end
end

-- Start the script
pcall(main)
